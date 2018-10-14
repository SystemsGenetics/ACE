#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include "../core/ace_logserver.h"



class QTcpSocket;
//



/*!
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
    */
   bool _silent {false};
   /*!
    */
   bool _isDebug {false};
   /*!
    */
   int _debugThread {0};
   /*!
    */
   int _waiting {0};
   /*!
    */
   QList<QTcpSocket*> _sockets;
};



#endif
