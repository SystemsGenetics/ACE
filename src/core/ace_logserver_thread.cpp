#include "ace_logserver_thread.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include "eexception.h"



using namespace Ace;
//






/*!
 *
 * @param port  
 *
 * @param parent  
 */
LogServer::Thread::Thread(int port, QObject* parent):
   QThread(parent),
   _port(port)
{}






/*!
 */
void LogServer::Thread::wait()
{
   while ( _ready == 0 );
}






/*!
 * Writes out the given message to all connected clients. 
 *
 * @param data Any optional data that is appended to the message. 
 *
 * @return Reference to this logger. 
 */
void LogServer::Thread::broadcast(const QByteArray& data)
{
   _lock.lock();
   _data = &data;
   ++_switch;
   while ( _switch == 1 );
   _lock.unlock();
}






/*!
 */
void LogServer::Thread::run()
{
   QTcpServer server;
   if ( !server.listen(QHostAddress::Any,_port) )
   {
      E_MAKE_EXCEPTION(e);
      e.setDetails(tr("System Error: Failed binding socket to listen on port %1: %2")
                   .arg(_port)
                   .arg(server.errorString()));
      throw e;
   }
   QVector<QTcpSocket*> clients;
   while (true)
   {
      QCoreApplication::processEvents();
      while ( server.hasPendingConnections() )
      {
         clients << server.nextPendingConnection();
      }
      for (auto i = clients.begin(); i != clients.end() ;)
      {
         QTcpSocket* client {*i};
         if ( client->state() != QTcpSocket::ConnectedState )
         {
            i = clients.erase(i);
            client->deleteLater();
         }
         else
         {
            if ( client->bytesAvailable() > 0 )
            {
               client->read(client->bytesAvailable());
               ++_ready;
            }
            ++i;
         }
      }
      if ( _switch == 1 )
      {
         for (auto client: clients)
         {
            client->write(*_data);
            client->waitForReadyRead(-1);
            client->read(client->bytesAvailable());
         }
         --_switch;
      }
      usleep(10);
      if ( _switch == 0 )
      {
         msleep(10);
      }
   }
}
