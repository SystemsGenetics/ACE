#include "ace_logserver.h"
#include <QHostInfo>
#include "ace_logserver_thread.h"
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
 */
void LogServer::wait()
{
   _server->wait();
}






/*!
 * Writes out the given message to all connected clients. 
 *
 * @param type The message type being broadcast to all clients. 
 *
 * @param thread  
 *
 * @param data Any optional data that is appended to the message. 
 *
 * @return Reference to this logger. 
 */
LogServer& LogServer::broadcast(Type type, int thread, const QByteArray& data)
{
   if ( data.isEmpty() )
   {
      return *this;
   }

   // Construct the message that will be broadcast to all connected clients. 
   QByteArray message;
   QDataStream stream(&message,QIODevice::WriteOnly);
   stream << (qint8)type << (qint32)thread << (qint32)data.size();
   stream.writeRawData(data.data(),data.size());

   _server->broadcast(message);
   return *this;
}






/*!
 *
 * @param port  
 */
LogServer::LogServer(int port)
{
   _server = new Thread(port,this);
   _server->start();
}
