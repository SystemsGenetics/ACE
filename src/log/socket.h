#ifndef SOCKET_H
#define SOCKET_H
#include <QTcpSocket>
#include "../core/ace_logserver.h"
//



/*!
 * This is the client logging socket for the ACE logging framework. This is derived 
 * from a qt TCP socket class with an additional signal when it receives a 
 * formatted logging server message. All other functionality required, such as 
 * connecting to a server, is in the base qt socket class. 
 */
class Socket : public QTcpSocket
{
   Q_OBJECT
public:
   Socket();
signals:
   /*!
    * Signals that the client has received a message from its connected logging 
    * server. 
    *
    * @param type The message type that was received. 
    *
    * @param thread The thread where the message originated on the ACE application 
    *               where the logging server resides. Only applicable for debug 
    *               messages. 
    *
    * @param message The message itself as a local 8 bit string. 
    */
   void messageReceived(Ace::LogServer::Type type, int thread, const QByteArray& message);
private slots:
   void readyRead();
private:
   void readHeader();
   void readMessage();
   /*!
    * The message type that is currently being processed by this socket. 
    */
   qint8 _type;
   /*!
    * The thread where the message currently being processed originated on the ACE 
    * application this client is connected to. 
    */
   qint32 _thread;
   /*!
    * The size of the message body currently being processed by this socket if 0 or 
    * greater. If this is -1 then this socket is processing the header of a new 
    * message. 
    */
   qint32 _sizeToRead {-1};
   /*!
    * The internal buffer of this socket that temporarily stores new message headers 
    * and bodies as they are processed. 
    */
   QByteArray _buffer;
};



#endif
