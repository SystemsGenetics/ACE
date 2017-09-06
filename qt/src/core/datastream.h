#ifndef EDATASTREAM_H
#define EDATASTREAM_H
#include <QtCore>
#include <memory>

#include "utilities.h"
#include "exception.h"



/// Data stream used for input/output of data objects. All output stream operators will do nothing
/// if the stream is in an error state and input stream operators will set their values to null.
class EDataStream : public ESilent
{
public:
   EDataStream(QFile* file);
   ACE_DISBALE_COPY_AND_MOVE(EDataStream)
   template<class T> bool write(const T* value, quint64 size = 1);
   template<class T> bool read(T* value, quint64 size = 1);
   EDataStream& operator<<(qint8 value) { return writeNumber(value); }
   EDataStream& operator<<(quint8 value) { return writeNumber(value); }
   EDataStream& operator<<(qint16 value) { return writeNumber(value); }
   EDataStream& operator<<(quint16 value) { return writeNumber(value); }
   EDataStream& operator<<(qint32 value) { return writeNumber(value); }
   EDataStream& operator<<(quint32 value) { return writeNumber(value); }
   EDataStream& operator<<(qint64 value) { return writeNumber(value); }
   EDataStream& operator<<(quint64 value) { return writeNumber(value); }
   EDataStream& operator<<(float value) { return writeFloat(value); }
   EDataStream& operator<<(double value) { return writeFloat(value); }
   EDataStream& operator<<(const QString& value);
   EDataStream& operator<<(const QByteArray& value);
   EDataStream& operator>>(qint8& value) { return readNumber(value); }
   EDataStream& operator>>(quint8& value) { return readNumber(value); }
   EDataStream& operator>>(qint16& value) { return readNumber(value); }
   EDataStream& operator>>(quint16& value) { return readNumber(value); }
   EDataStream& operator>>(qint32& value) { return readNumber(value); }
   EDataStream& operator>>(quint32& value) { return readNumber(value); }
   EDataStream& operator>>(qint64& value) { return readNumber(value); }
   EDataStream& operator>>(quint64& value) { return readNumber(value); }
   EDataStream& operator>>(float& value) { return readFloat(value); }
   EDataStream& operator>>(double& value) { return readFloat(value); }
   EDataStream& operator>>(QString& value);
   EDataStream& operator>>(QByteArray& value);
private:
   enum
   {
      String = 85
      ,ByteArray = 170
   };
   template<class T> EDataStream& writeNumber(T& value);
   template<class T> EDataStream& writeFloat(T& value);
   template<class T> EDataStream& readNumber(T& value);
   template<class T> EDataStream& readFloat(T& value);
   QFile* _file;
};






template<class T>
bool EDataStream::write(const T* value, quint64 size)
{
   // write data to file
   if ( static_cast<quint64>(_file->write(reinterpret_cast<const char*>(value),sizeof(T)*size))
        != sizeof(T)*size )
   {
      // if write failed report error and return false
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Data Stream Write"));
      e.setDetails(QObject::tr("Failed writing to file: %1").arg(_file->errorString()));
      setException(e);
      return false;
   }

   // return true on write success
   return true;
}






template<class T>
bool EDataStream::read(T* value, quint64 size)
{
   // read data from file
   if ( static_cast<quint64>(_file->read(reinterpret_cast<char*>(value),sizeof(T)*size))
        != sizeof(T)*size )
   {
      // if read failed report error and return false
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Data Stream Read"));
      e.setDetails(QObject::tr("Failed reading from file: %1").arg(_file->errorString()));
      setException(e);
      return false;
   }

   // return true on read success
   return true;
}






template<class T>
EDataStream& EDataStream::writeNumber(T& value)
{
   // make sure stream is in ok state
   if ( !*this )
   {
      return *this;
   }

   // change endianness to file format if required
   if ( sizeof(T) > 1 )
   {
      value = qToBigEndian(value);
   }

   // write to stream and return reference to stream
   write(&value);
   return *this;
}






template<class T>
EDataStream& EDataStream::writeFloat(T& value)
{
   // make sure stream is in ok state
   if ( !*this )
   {
      return *this;
   }

   // write to stream and return reference to stream
   write(&value);
   return *this;
}






template<class T>
EDataStream& EDataStream::readNumber(T& value)
{
   // make sure stream is in ok state
   if ( !*this )
   {
      value = 0;
      return *this;
   }

   // read value from stream
   if ( read(&value) && sizeof(T) > 1 )
   {
      // if read successfull and endian matters convert endianness to local
      value = qFromBigEndian(value);
   }

   // return reference to stream
   return *this;
}






template<class T>
EDataStream& EDataStream::readFloat(T& value)
{
   // make sure stream is in ok state
   if ( !*this )
   {
      value = 0.0;
      return *this;
   }

   // read value from stream
   read(&value);

   // return reference to stream
   return *this;
}



#endif
