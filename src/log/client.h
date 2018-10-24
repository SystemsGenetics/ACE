#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include "../core/ace_logserver.h"



class QTcpSocket;
//



/*!
 * This is the main client that manages all open socket connections to logging 
 * servers and outputs the messages it is configured to listen to to standard 
 * output. It also handles configuration by parsing any valid options given as 
 * command line arguments. 
 */
class Client : public QObject
{
   Q_OBJECT
public:
   explicit Client();
private slots:
   void messageReceived(int socket, Ace::LogServer::Type type, int thread, const QByteArray& message);
   void connected();
   void disconnected();
private:
   /*!
    * Holds the silent option for this client. True will cause the client to not send 
    * the start signal once it connects to all logging servers. 
    */
   bool _silent {false};
   /*!
    * Holds the debug option for this client. True means this client will be listening 
    * to debug messages instead of normal logging messages. This also means it can 
    * only be connected to one logging server. 
    */
   bool _isDebug {false};
   /*!
    * Holds which thread this client listens to if and only if it is in debug mode. 
    * Any debug message not part of this thread will be ignored by this client. 
    */
   int _debugThread {0};
   /*!
    * The total number of sockets of this client that are not yet connected. This is 
    * used to synchronize the start signal after all sockets are connected. 
    */
   int _waiting {0};
   /*!
    * List of sockets connected to logging servers that this client manages. 
    */
   QList<QTcpSocket*> _sockets;
};



#endif
