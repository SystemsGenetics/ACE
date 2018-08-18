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
 * Writes out the given byte array to this logger and all its connected clients. 
 *
 * @param data The data that is relayed by this logger to all clients and saved in 
 *             its buffer for new clients. 
 *
 * @return Reference to this logger. 
 */
LogServer& LogServer::operator<<(const QByteArray& data)
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

      // Else the client is still connected so write out the given byte array to its TCP 
      // socket. 
      else
      {
         client->write(data);
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
 * Called when there is a new connection established for this log server from a new 
 * client. 
 */
void LogServer::newConnectionMade()
{
   // Continue while there are still pending connections, locking this log server's 
   // mutex for thread safety. 
   while ( hasPendingConnections() )
   {
      // Add the new client to this log server's list of clients, using its mutex for 
      // thread safety. 
      _mutex.lock();
      _clients << nextPendingConnection();
      _mutex.unlock();

      // If the is the first connection made to the log server then emit the first 
      // connection signal. 
      if ( !_first )
      {
         _first = true;
         emit firstConnection();
      }
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
