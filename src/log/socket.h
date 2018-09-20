#ifndef SOCKET_H
#define SOCKET_H
#include <QTcpSocket>
#include "../core/ace_logserver.h"
//



/*!
 */
class Socket : public QTcpSocket
{
   Q_OBJECT
signals:
   /*!
    *
    * @param type  
    *
    * @param thread  
    *
    * @param message  
    */
   void messageReceived(Ace::LogServer::Types type, int thread, const QByteArray& message);
private slots:
   void readyRead();
private:
   void readHeader();
   void readMessage();
   /*!
    */
   qint8 _type;
   /*!
    */
   qint32 _thread;
   /*!
    */
   qint32 _sizeToRead {-1};
   /*!
    */
   QByteArray _buffer;
};



#endif
