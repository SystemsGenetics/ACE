#include "ace_logserver.h"
#include <QTcpSocket>
#include <QHostInfo>
#include "eexception.h"
#include "ace_qmpi.h"
#include "ace_settings.h"



using namespace Ace;
//



/*!
 * Pointer to the global instance of this application's log server. 
 */
LogServer* LogServer::_log {nullptr};






/*!
 * Writes out the given message to all connected clients. 
 *
 * @param type The message type being broadcast to all clients. 
 *
 * @param data Any optional data that is appended to the message. 
 *
 * @return Reference to this logger. 
 */
LogServer& LogServer::broadcast(int type, const QByteArray& data)
{
   // Construct the message that will be broadcast to all connected clients. 
   QByteArray message;
   QDataStream stream(&message,QIODevice::WriteOnly);
   stream << (qint32)type << (qint32)data.size();
   stream.writeBytes(data.data(),data.size());

   // Iterate through all connected clients of this log server, locking its mutex for 
   // thread safety. 
   _mutex.lock();
   for (auto i = _clients.begin(); i != _clients.end() ;++i)
   {
      // Get the TCP socket of the client. 
      QTcpSocket* client {*i};

      // Check to see if this TCP socket is no longer connected. 
      if ( client->state() != QTcpSocket::ConnectedState )
      {
         // Remove the client from this log server and delete its TCP socket. 
         i = _clients.erase(i);
         client->deleteLater();
      }

      // Else the client is still connected so write out the given byte array to its TCP 
      // socket. 
      else
      {
         client->write(message);
      }
   }

   // Unlock this log server's mutex. 
   _mutex.unlock();

   // Return a reference to this log server. 
   return *this;
}






/*!
 * Creates and initializes the global instance of the log server. 
 *
 * @param port The port number the global log server listens for new connections. 
 */
void LogServer::initialize(int port)
{
   // Make sure the logging system was not already initialized. 
   if ( _log )
   {
      E_MAKE_EXCEPTION(e);
      e.setDetails(tr("Logging system was already initialized."));
      throw e;
   }

   // Create the global instance log server. 
   _log = new LogServer(port);
}






/*!
 * Returns the global instance of the logging server or null if it was never 
 * initialized. 
 *
 * @return Global instance of the logging server or null if it was never 
 *         initialized. 
 */
LogServer* LogServer::log()
{
   return _log;
}






/*!
 * Returns the hostname of the machine this program is running on. 
 *
 * @return Hostname of the machine this program is running on. 
 */
QString LogServer::host()
{
   return QHostInfo::localHostName();
}






/*!
 * Flushes all buffered output of all connect client sockets of this log server. 
 */
void LogServer::flush()
{
   // Iterate through all connected clients of this log server, locking its mutex for 
   // thread safety. 
   _mutex.lock();
   for (auto i = _clients.begin(); i != _clients.end() ;++i)
   {
      // Get the TCP socket of the client. 
      QTcpSocket* client {*i};

      // Check to see if this TCP socket is no longer connected. 
      if ( client->state() != QTcpSocket::ConnectedState )
      {
         // Remove the client from this log server and delete its TCP socket. 
         i = _clients.erase(i);
         client->deleteLater();
      }

      // Else the client is still connected so flush its TCP socket. 
      else
      {
         client->flush();
      }
   }

   // Unlock this log server's mutex. 
   _mutex.unlock();
}






/*!
 * Called when a connected client has sent data that signals the ACE program should 
 * start execution. 
 */
void LogServer::clientSignalsStart()
{
   // If the start signal has not yet been emitted then emit it. 
   if ( !_first )
   {
      _first = true;
      emit readyToStart();
   }
}






/*!
 * Called when there is a new connection established for this log server from a new 
 * client. 
 */
void LogServer::newConnectionMade()
{
   // Continue while there are still pending connections, locking this log server's 
   // mutex for thread safety. 
   while ( hasPendingConnections() )
   {
      // Add the new client to this log server's list of clients and connect its signal 
      // start signal, using its mutex for thread safety. 
      _mutex.lock();
      _clients << nextPendingConnection();
      connect(_clients.back(),&QTcpSocket::readyRead,this,&LogServer::clientSignalsStart);
      _mutex.unlock();
   }
}






/*!
 * Constructs a new logging server listening on the given port. 
 *
 * @param port The port number this log server listens for new connections. 
 */
LogServer::LogServer(int port)
{
   // Bind a socket to start listening on the given port with any address, making 
   // sure it worked. 
   if ( !listen(QHostAddress::Any,port) )
   {
      E_MAKE_EXCEPTION(e);
      e.setDetails(tr("System Error: Failed binding socket to listen on port %1: %2")
                   .arg(port)
                   .arg(errorString()));
      throw e;
   }

   // Connect the new connection signal. 
   connect(this,&QTcpServer::newConnection,this,&LogServer::newConnectionMade);
}
