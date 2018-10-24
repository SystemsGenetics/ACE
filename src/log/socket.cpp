#include "socket.h"
#include <QDataStream>
#include <QTimer>



//






/*!
 * Constructs a new socket object. 
 */
Socket::Socket()
{
   connect(this,&QIODevice::readyRead,this,&Socket::readyRead);
}






/*!
 * Called when this object's underlying qt IO device has new bytes that is ready to 
 * be read. This processes those bytes as incoming messages from the logging 
 * server. 
 */
void Socket::readyRead()
{
   // If this object is in the process of reading in a message header then call the 
   // appropriate processing method. 
   if ( _sizeToRead < 0 )
   {
      readHeader();
   }

   // Else this object is in the process of reading in the body of a message so call 
   // the processing method for doing it. 
   else
   {
      readMessage();
   }
}






/*!
 * Processes any available bytes as a new header for a message until a full header 
 * has been processed. 
 */
void Socket::readHeader()
{
   // Keep reading in bytes from the socket until the full header has been read in to 
   // the internal buffer or there are is no more bytes to read. 
   while ( _buffer.size() < 9 && bytesAvailable() )
   {
      _buffer += read(1);
   }

   // Check to see if a full message header has been read into the internal buffer. 
   if ( _buffer.size() == 9 )
   {
      // Read in the header message contents and clear the internal buffer. 
      QDataStream in(_buffer);
      in >> _type >> _thread >> _sizeToRead;
      _buffer.clear();

      // If there are still more bytes to read then call the ready read slot indirectly. 
      // This must be done instead of directly calling the method to avoid making the 
      // function stack too large. 
      if ( bytesAvailable() > 0 )
      {
         QTimer::singleShot(0,this,&Socket::readyRead);
      }
   }
}






/*!
 * Processes any available bytes as a new body for a message until the full body 
 * has been processed. 
 */
void Socket::readMessage()
{
   // Keep reading in bytes from the socket until the full body has been read in to 
   // the internal buffer or there are no more bytes to read. 
   while ( _buffer.size() < _sizeToRead && bytesAvailable() )
   {
      _buffer += read(1);
   }

   // Check to see if the full message body has been read into the internal buffer. 
   if ( _buffer.size() == _sizeToRead )
   {
      // Emit the message received signal. 
      emit messageReceived(static_cast<Ace::LogServer::Type>(_type),_thread,_buffer);

      // Write back to the server on the socket signaling the message has been received. 
      write("A",1);
      flush();

      // Reset the internal buffer and set the size to read state to processing a new 
      // message header. 
      _buffer.clear();
      _sizeToRead = -1;

      // If there are more bytes to read then call the ready read slot indirectly. 
      if ( bytesAvailable() > 0 )
      {
         QTimer::singleShot(0,this,&Socket::readyRead);
      }
   }
}
