#include <QtEndian>
#include <QPixmap>

#include "datastream.h"



using namespace std;






EDataStream::EDataStream(QFile *file):
   _file(file)
{}






EDataStream& EDataStream::operator<<(qint8 value)
{
   return writeNumber(value);
}






EDataStream& EDataStream::operator<<(quint8 value)
{
   return writeNumber(value);
}






EDataStream& EDataStream::operator<<(qint16 value)
{
   return writeNumber(value);
}






EDataStream& EDataStream::operator<<(quint16 value)
{
   return writeNumber(value);
}






EDataStream& EDataStream::operator<<(qint32 value)
{
   return writeNumber(value);
}






EDataStream& EDataStream::operator<<(quint32 value)
{
   return writeNumber(value);
}






EDataStream& EDataStream::operator<<(qint64 value)
{
   return writeNumber(value);
}






EDataStream& EDataStream::operator<<(quint64 value)
{
   return writeNumber(value);
}






EDataStream& EDataStream::operator<<(float value)
{
   return writeFloat(value);
}






EDataStream& EDataStream::operator<<(double value)
{
   return writeFloat(value);
}






EDataStream& EDataStream::operator<<(const QString& value)
{
   // make sure stream is in ok state
   if ( *this )
   {
      // write out string identifier type
      quint8 type = String;
      if ( write(&type) )
      {
         // write out string in UTF8 format
         *this << value.toUtf8();
      }
   }

   // return reference to stream
   return *this;
}






EDataStream& EDataStream::operator<<(const QByteArray& value)
{
   // make sure stream is in ok state
   if ( *this )
   {
      // write out byte array identifier type
      quint8 type = ByteArray;
      if ( write(&type) )
      {
         // write out size of byte array
         quint32 size = qToBigEndian((quint32)value.size());
         if ( write(&size) )
         {
            // write out byte array
            write(value.data(),value.size());
         }
      }
   }

   // return reference to stream
   return *this;
}






EDataStream& EDataStream::operator>>(qint8& value)
{
   return readNumber(value);
}






EDataStream& EDataStream::operator>>(quint8& value)
{
   return readNumber(value);
}






EDataStream& EDataStream::operator>>(qint16& value)
{
   return readNumber(value);
}






EDataStream& EDataStream::operator>>(quint16& value)
{
   return readNumber(value);
}






EDataStream& EDataStream::operator>>(qint32& value)
{
   return readNumber(value);
}






EDataStream& EDataStream::operator>>(quint32& value)
{
   return readNumber(value);
}






EDataStream& EDataStream::operator>>(qint64& value)
{
   return readNumber(value);
}






EDataStream& EDataStream::operator>>(quint64& value)
{
   return readNumber(value);
}






EDataStream& EDataStream::operator>>(float& value)
{
   return readFloat(value);
}






EDataStream& EDataStream::operator>>(double& value)
{
   return readFloat(value);
}






EDataStream& EDataStream::operator>>(QString& value)
{
   // clear input string
   value.clear();

   // make sure stream is in ok state
   if ( *this )
   {
      // read type identifier
      quint8 type {0};
      if ( read(&type) )
      {
         // make sure it is correct type
         if ( type != String )
         {
            E_MAKE_EXCEPTION(e);
            e.setLevel(EException::Critical);
            e.setType(CorruptData);
            e.setTitle(QObject::tr("Data Stream Read"));
            e.setDetails(QObject::tr("Could not read string because data is corrupt"));
            setException(e);
         }
         else
         {
            // read in string in UTF8 format
            QByteArray data;
            *this >> data;
            value = QString::fromUtf8(data);
         }
      }
   }

   // return reference to stream
   return *this;
}






EDataStream& EDataStream::operator>>(QByteArray& value)
{
   // clear input
   value.clear();

   // make sure stream is in ok state
   if ( *this )
   {
      // read type identifier
      quint8 type {0};
      if ( read(&type) )
      {
         // make sure it is correct type
         if ( type != ByteArray )
         {
            E_MAKE_EXCEPTION(e);
            e.setLevel(EException::Critical);
            e.setType(CorruptData);
            e.setTitle(QObject::tr("Data Stream Read"));
            e.setDetails(QObject::tr("Could not read byte array because data is corrupt."));
            setException(e);
         }
         else
         {
            // read size of byte array
            quint32 size;
            if ( read(&size) )
            {
               size = qFromBigEndian(size);

               // read byte array and make sure it worked
               value.resize(size);
               if ( !read(value.data(),size) )
               {
                  E_MAKE_EXCEPTION(e);
                  e.setLevel(EException::Critical);
                  e.setType(CorruptData);
                  e.setTitle(QObject::tr("Data Stream Read"));
                  e.setDetails(QObject::tr("Could not read byte array because data is corrupt."));
                  setException(e);
               }
            }
         }
      }
   }

   // return reference to stream
   return *this;
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






template<class T>
bool EDataStream::write(T* value, quint64 size)
{
   // write data to file
   if ( static_cast<quint64>(_file->write(reinterpret_cast<const char*>(value),sizeof(T)*size))
        != sizeof(T)*size )
   {
      // if write failed report error and return false
      E_MAKE_EXCEPTION(e);
      e.setLevel(EException::Critical);
      e.setType(WriteFailed);
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
      e.setLevel(EException::Critical);
      e.setType(ReadFailed);
      e.setTitle(QObject::tr("Data Stream Read"));
      e.setDetails(QObject::tr("Failed reading from file: %1").arg(_file->errorString()));
      setException(e);
      return false;
   }

   // return true on read success
   return true;
}
