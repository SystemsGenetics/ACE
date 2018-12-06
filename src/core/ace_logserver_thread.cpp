#include "ace_logserver_thread.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include "eexception.h"



using namespace Ace;
//






/*!
 * Constructs a new thread instance with the given listening port and optional 
 * parent. This does NOT start the TCP server. 
 *
 * @param port The port number this new thread's internal TCP server will listen on 
 *             for client connections. 
 *
 * @param parent Optional parent for this new thread. 
 */
LogServer::Thread::Thread(int port, QObject* parent):
   QThread(parent),
   _port(port)
{}






/*!
 * Blocks execution of the calling thread until a connected logging client signals 
 * the ACE application to start. 
 */
void LogServer::Thread::wait()
{
   while ( _ready == 0 );
}






/*!
 * Writes out the given message to all connected clients on this thread's internal 
 * TCP server. 
 *
 * @param data Any optional data that is appended to the message. 
 */
void LogServer::Thread::broadcast(const QByteArray& data)
{
   // Lock this thread's mutex. 
   _lock.lock();

   // Set this thread's data pointer to the given byte array and increment the switch 
   // atomic integer, signaling that data is ready to be broadcast. 
   _data = &data;
   ++_switch;

   // Wait until the switch has been lowered, signaling the data was broadcast to all 
   // clients. 
   while ( _switch == 1 );

   // Unlock this thread's mutex. 
   _lock.unlock();
}






/*!
 * Implements _QThread::run_. 
 */
void LogServer::Thread::run()
{
   // Initialize the TCP server and start listening on the given port, making sure it 
   // worked. 
   QTcpServer server;
   if ( !server.listen(QHostAddress::Any,_port) )
   {
      qDebug().noquote() << tr("System Error: Failed binding socket to listen on port %1: %2")
                            .arg(_port)
                            .arg(server.errorString());
      return;
   }

   // Initialize the list of connected clients. 
   QVector<QTcpSocket*> clients;

   // Begin the infinite loop that processing all TCP events. 
   while (true)
   {
      // Process all events for this thread. 
      QCoreApplication::processEvents();

      // Add any new client connections to the list of connected clients. 
      while ( server.hasPendingConnections() )
      {
         clients << server.nextPendingConnection();
      }

      // Iterate through all connects clients. 
      for (auto i = clients.begin(); i != clients.end() ;)
      {
         // Check to see if the client is still connected, removing them from the list if 
         // it is no longer connected. 
         QTcpSocket* client {*i};
         if ( client->state() != QTcpSocket::ConnectedState )
         {
            i = clients.erase(i);
            client->deleteLater();
         }

         // Else the client connection is still active. 
         else
         {
            // Check to see if the client sent a start signal, incrementing the ready switch 
            // if so. 
            if ( client->bytesAvailable() > 0 )
            {
               client->read(client->bytesAvailable());
               ++_ready;
            }

            // Move to the next connected client socket. 
            ++i;
         }
      }

      // Check to see if a data packet is waiting to be broadcast. 
      if ( _switch == 1 )
      {
         // Iterate through all connected clients. 
         for (auto client: clients)
         {
            // Send the data message to the client, waiting until they send a confirmation 
            // back. 
            client->write(*_data);
            client->waitForReadyRead(-1);
            client->read(client->bytesAvailable());
         }

         // Decrement the switch signaling the data packet has been broadcast. 
         --_switch;
      }

      // Sleep for a very short amount of time, check to see if a data packet is 
      // waiting, and sleep a much longer period of time if none is waiting. 
      usleep(10);
      if ( _switch == 0 )
      {
         msleep(10);
      }
   }
}
