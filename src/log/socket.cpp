#include "socket.h"
#include <QDataStream>



//






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
   _buffer += read(qMin((qint64)9 - _buffer.size(),bytesAvailable()));
   if ( _buffer.size() == 9 )
   {
      QDataStream in(_buffer);
      in >> _type >> _thread >> _sizeToRead;
      _buffer.clear();
      if ( bytesAvailable() > 0 )
      {
         readMessage();
      }
   }
}






/*!
 */
void Socket::readMessage()
{
   _buffer += read(qMin((qint64)_sizeToRead - _buffer.size(),bytesAvailable()));
   if ( _buffer.size() == _sizeToRead )
   {
      emit messageReceived(static_cast<Ace::LogServer::Types>(_type),_thread,_buffer);
      _buffer.clear();
      _sizeToRead = -1;
      if ( bytesAvailable() > 0 )
      {
         readHeader();
      }
   }
}
