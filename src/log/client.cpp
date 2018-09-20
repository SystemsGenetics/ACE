#include "client.h"
#include <QCommandLineParser>



//






/*!
 *
 * @param argc The command line argument size passed from the main function. 
 *
 * @param argv The command line arguments passed from the main function. 
 */
Client::Client(int& argc, char** argv)
{}






/*!
 *
 * @param socket  
 *
 * @param type  
 *
 * @param thread  
 *
 * @param message  
 */
void Client::messageReceived(int socket, Ace::LogServer::Types type, int thread, const QByteArray& message)
{
   if ( _isDebug && type == Ace::LogServer::Debug )
   {
      if ( thread == _debugThread )
      {
         QTextStream out(stdout);
         out << message << QStringLiteral("\n");
      }
   }
   else if ( !_isDebug && type == Ace::LogServer::Log )
   {
      QTextStream out(stdout);
      out << QStringLiteral("[")
          << socket
          << QStringLiteral("] ")
          << message
          << QStringLiteral("\n");
   }
}






/*!
 */
void Client::disconnected()
{
   deleteLater();
}
