#include "client.h"
#include <QCoreApplication>
#include <QCommandLineParser>
#include "socket.h"



//






/*!
 * Constructs a new client. This first parses any command line options to configure 
 * itself and then connects to any given hosts for listening. 
 */
Client::Client()
{
   // Create a command line parser and add the command line options this new client 
   // is interested in. 
   QCommandLineParser parse;
   parse.addOptions(
   {
      {{"s","silent"},tr("Do not signal the server to start.")}
      ,{{"d","debug"},tr("Run in debug mode.")}
      ,{{"t","thread"},tr("The given thread number to listen to in debug mode."),"thread"}
   }
   );

   // Parse the command line arguments. 
   parse.process(*QCoreApplication::instance());

   // Set the silent and debug options. 
   _silent = parse.isSet("silent");
   _isDebug = parse.isSet("debug");

   // Set the thread option if it is set. 
   if ( parse.isSet("thread") )
   {
      _debugThread = parse.value("thread").toInt();
   }

   // Iterate through all remaining arguments that should be host:port logging server 
   // targets. 
   const QStringList list {parse.positionalArguments()};
   for (auto argument: list)
   {
      // Split the argument and make sure it is valid. 
      QStringList split {argument.split(':')};
      if ( split.size() == 2 )
      {
         // Get the port number and make sure it is valid. 
         bool ok;
         int port {split.at(1).toInt(&ok)};
         if ( ok )
         {
            // Create a new socket, add it to this client's list of sockets, and increment 
            // this client's waiting amount. 
            Socket* socket {new Socket(this)};
            _sockets << socket;
            int i {_waiting++};

            // Connect all relevant signals of the new socket to this client. 
            connect(socket,&QTcpSocket::connected,this,&Client::connected);
            connect(socket,&QTcpSocket::disconnected,this,&Client::disconnected);
            connect(socket,&Socket::messageReceived
                    ,[this,i](Ace::LogServer::Type type, int thread, const QByteArray& message)
                    { messageReceived(i,type,thread,message); }
            );

            // Connect to the logging server with the new socket. 
            socket->connectToHost(split.at(0),port);

            // If the debug option is set then stop processing logging server arguments 
            // because debugging can only connect to one logging server. 
            if ( _isDebug && _waiting >= 1 )
            {
               return;
            }
         }
      }
   }
}






/*!
 * Called when one of this client's connected sockets has received a logging server 
 * message. This processes the message and prints it to standard output if it 
 * matches what this client is listening for. 
 *
 * @param socket The socket the emitted its message received signal. 
 *
 * @param type The message type that was received. 
 *
 * @param thread The thread where the message originated on the ACE application 
 *               where the logging server resides. Only applicable for debug 
 *               messages. 
 *
 * @param message The message itself as a local 8 bit string. 
 */
void Client::messageReceived(int socket, Ace::LogServer::Type type, int thread, const QByteArray& message)
{
   // Check if this is a debugging message and this client is listening to debug 
   // messages. 
   if ( _isDebug && type == Ace::LogServer::Debug )
   {
      // If the debug message matches the thread this client is listening for then 
      // output it to standard output. 
      if ( thread == _debugThread )
      {
         QTextStream out(stdout);
         out << message << QStringLiteral("\n");
      }
   }

   // Else check to see if this is a logging message and this client is listening to 
   // logging messages. 
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
 * Called when one of this client's sockets has successfully connected to its 
 * remote logging server. This keeps track of how many remaining sockets are 
 * waiting for connection and fires the start signal once they are all connected if 
 * the client is configured to do so. 
 */
void Client::connected()
{
   // Decrement this client's sockets waiting variable. 
   --_waiting;

   // If all sockets are now connected and this client is configured not to be silent 
   // then iterate through all sockets and send the start signal to their connected 
   // logging servers. 
   if ( _waiting == 0 && !_silent )
   {
      for (auto socket: qAsConst(_sockets))
      {
         socket->write("S",1);
         socket->flush();
      }
   }
}






/*!
 * Called when one of this client's sockets got disconnected. This simply exits the 
 * application and deleting this client. 
 */
void Client::disconnected()
{
   deleteLater();
}
