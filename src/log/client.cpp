#include "client.h"
#include <QCoreApplication>
#include <QCommandLineParser>
#include "socket.h"



//






/*!
 */
Client::Client()
{
   QCommandLineParser parse;
   parse.addOptions(
   {
      {{"s","silent"},tr("Do not signal the server to start.")}
      ,{{"d","debug"},tr("Run in debug mode.")}
      ,{{"t","thread"},tr("The given thread number to listen to in debug mode.")}
   }
   );
   parse.process(*QCoreApplication::instance());
   _silent = parse.isSet("silent");
   _isDebug = parse.isSet("debug");
   if ( parse.isSet("thread") )
   {
      _debugThread = parse.value("thread").toInt();
   }
   const QStringList list {parse.positionalArguments()};
   for (auto argument: list)
   {
      QStringList split {argument.split(':')};
      if ( split.size() == 2 )
      {
         bool ok;
         int port {split.at(1).toInt(&ok)};
         if ( ok )
         {
            Socket* socket {new Socket};
            _sockets << socket;
            int i {_waiting++};
            connect(socket,&QTcpSocket::connected,this,&Client::connected);
            connect(socket,&QTcpSocket::disconnected,this,&Client::disconnected);
            connect(socket,&Socket::messageReceived
                    ,[this,i](Ace::LogServer::Type type, int thread, const QByteArray& message)
                    { messageReceived(i,type,thread,message); }
            );
            socket->connectToHost(split.at(0),port);
            if ( _isDebug && _waiting >= 1 )
            {
               return;
            }
         }
      }
   }
}






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
void Client::messageReceived(int socket, Ace::LogServer::Type type, int thread, const QByteArray& message)
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
void Client::connected()
{
   --_waiting;
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
 */
void Client::disconnected()
{
   deleteLater();
}
