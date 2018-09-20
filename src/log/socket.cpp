#include "socket.h"
#include <QDataStream>
#include <QTimer>



//


Socket::Socket()
{
   connect(this,&QIODevice::readyRead,this,&Socket::readyRead);
}



/*!
 */
void Socket::readyRead()
{
   if ( _sizeToRead < 0 )
   {
      readHeader();
   }
   else
   {
      readMessage();
   }
}






/*!
 */
void Socket::readHeader()
{
   while ( _buffer.size() < 9 && bytesAvailable() )
   {
      _buffer += read(1);
   }
   if ( _buffer.size() == 9 )
   {
      QDataStream in(_buffer);
      in >> _type >> _thread >> _sizeToRead;
      _buffer.clear();
      if ( bytesAvailable() > 0 )
      {
         QTimer::singleShot(0,this,&Socket::readyRead);
      }
   }
}






/*!
 */
void Socket::readMessage()
{
   while ( _buffer.size() < _sizeToRead && bytesAvailable() )
   {
      _buffer += read(1);
   }
   if ( _buffer.size() == _sizeToRead )
   {
      emit messageReceived(static_cast<Ace::LogServer::Type>(_type),_thread,_buffer);
      write("A",1);
      flush();
      _buffer.clear();
      _sizeToRead = -1;
      if ( bytesAvailable() > 0 )
      {
         QTimer::singleShot(0,this,&Socket::readyRead);
      }
   }
}
