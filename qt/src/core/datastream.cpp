#include <QtEndian>
#include <QPixmap>

#include "datastream.h"



using namespace std;






EDataStream::EDataStream(QFile *file):
   _file(file)
{}






EDataStream::Status EDataStream::getStatus() const
{
   return _status;
}






EDataStream::operator bool() const
{
   return _status == Ok;
}






EDataStream& EDataStream::operator<<(qint8 value)
{
   if ( _status == Ok )
   {
      write(value);
   }
   return *this;
}






EDataStream& EDataStream::operator<<(quint8 value)
{
   if ( _status == Ok )
   {
      write(value);
   }
   return *this;
}






EDataStream& EDataStream::operator<<(qint16 value)
{
   if ( _status == Ok )
   {
      value = qToBigEndian(value);
      write(value);
   }
   return *this;
}






EDataStream& EDataStream::operator<<(quint16 value)
{
   if ( _status == Ok )
   {
      value = qToBigEndian(value);
      write(value);
   }
   return *this;
}






EDataStream& EDataStream::operator<<(qint32 value)
{
   if ( _status == Ok )
   {
      value = qToBigEndian(value);
      write(value);
   }
   return *this;
}






EDataStream& EDataStream::operator<<(quint32 value)
{
   if ( _status == Ok )
   {
      value = qToBigEndian(value);
      write(value);
   }
   return *this;
}






EDataStream& EDataStream::operator<<(qint64 value)
{
   if ( _status == Ok )
   {
      value = qToBigEndian(value);
      write(value);
   }
   return *this;
}






EDataStream& EDataStream::operator<<(quint64 value)
{
   if ( _status == Ok )
   {
      value = qToBigEndian(value);
      write(value);
   }
   return *this;
}






EDataStream& EDataStream::operator<<(float value)
{
   if ( _status == Ok )
   {
      write(value);
   }
   return *this;
}






EDataStream& EDataStream::operator<<(double value)
{
   if ( _status == Ok )
   {
      write(value);
   }
   return *this;
}






EDataStream& EDataStream::operator<<(const QString& value)
{
   if ( _status == Ok )
   {
      if ( value.size() > _maxStringSize )
      {
         _status = Status::StringTooBig;
         return *this;
      }
      quint8 type = String;
      if ( write(type) )
      {
         quint16 size = qToBigEndian(value.size());
         if ( write(size) )
         {
            write(value.data(),size);
         }
      }
   }
   return *this;
}






EDataStream& EDataStream::operator<<(const QPixmap& value)
{
   if ( _status == Ok )
   {
      quint8 type = Pixmap;
      if ( write(type) )
      {
         QByteArray data;
         QBuffer buffer(&data);
         buffer.open(QIODevice::WriteOnly);
         value.save(&buffer,"PNG");
         quint32 size = qToBigEndian(data.size());
         if ( write(size) )
         {
            write(data.data(),size);
         }
      }
   }
   return *this;
}






EDataStream& EDataStream::operator>>(qint8& value)
{
   value = 0;
   if ( _status == Ok )
   {
      read(&value);
   }
   return *this;
}






EDataStream& EDataStream::operator>>(quint8& value)
{
   value = 0;
   if ( _status == Ok )
   {
      read(&value);
   }
   return *this;
}






EDataStream& EDataStream::operator>>(qint16& value)
{
   value = 0;
   if ( _status == Ok )
   {
      if ( read(&value) )
      {
         value = qFromBigEndian(value);
      }
   }
   return *this;
}






EDataStream& EDataStream::operator>>(quint16& value)
{
   value = 0;
   if ( _status == Ok )
   {
      if ( read(&value) )
      {
         value = qFromBigEndian(value);
      }
   }
   return *this;
}






EDataStream& EDataStream::operator>>(qint32& value)
{
   value = 0;
   if ( _status == Ok )
   {
      if ( read(&value) )
      {
         value = qFromBigEndian(value);
      }
   }
   return *this;
}






EDataStream& EDataStream::operator>>(quint32& value)
{
   value = 0;
   if ( _status == Ok )
   {
      if ( read(&value) )
      {
         value = qFromBigEndian(value);
      }
   }
   return *this;
}






EDataStream& EDataStream::operator>>(qint64& value)
{
   value = 0;
   if ( _status == Ok )
   {
      if ( read(&value) )
      {
         value = qFromBigEndian(value);
      }
   }
   return *this;
}






EDataStream& EDataStream::operator>>(quint64& value)
{
   value = 0;
   if ( _status == Ok )
   {
      if ( read(&value) )
      {
         value = qFromBigEndian(value);
      }
   }
   return *this;
}






EDataStream& EDataStream::operator>>(float& value)
{
   value = 0.0;
   if ( _status == Ok )
   {
      read(&value);
   }
   return *this;
}






EDataStream& EDataStream::operator>>(double& value)
{
   value = 0.0;
   if ( _status == Ok )
   {
      read(&value);
   }
   return *this;
}






EDataStream& EDataStream::operator>>(QString& value)
{
   value.clear();
   if ( _status == Ok )
   {
      quint8 type {0};
      if ( read(&type) )
      {
         if ( type == String )
         {
            quint16 size;
            if ( read(&size) )
            {
               size = qFromBigEndian(size);
               QChar* buffer;
               unique_ptr<QChar> holder;
               if ( (size+1) > _stringBufferSize )
               {
                  holder = unique_ptr<QChar>(new QChar[size+1]);
                  buffer = holder.get();
               }
               else
               {
                  buffer = _stringBuffer;
               }
               if ( read(buffer,size) )
               {
                  buffer[size] = QChar('\0');
                  value = QString(buffer,size);
               }
            }
         }
         else
         {
            _status = Status::CorruptData;
         }
      }
   }
   return *this;
}






EDataStream& EDataStream::operator>>(QPixmap& value)
{
   value = QPixmap();
   if ( _status == Ok )
   {
      quint8 type {0};
      if ( read(&type) )
      {
         if ( type == Pixmap )
         {
            quint32 size;
            if ( read(&size) )
            {
               size = qFromBigEndian(size);
               QByteArray buffer;
               buffer.resize(size);
               if ( read(buffer.data(),size) )
               {
                  if ( !value.loadFromData(buffer) )
                  {
                     _status = Status::CorruptData;
                  }
               }
            }
         }
         else
         {
            _status = Status::CorruptData;
         }
      }
   }
   return *this;
}






template<class T>
bool EDataStream::write(T value, quint64 size)
{
   if ( static_cast<quint64>(_file->write(reinterpret_cast<char*>(&value),sizeof(T)*size))
        != sizeof(T)*size )
   {
      _status = Status::WriteFailed;
      return false;
   }
   return true;
}






template<class T>
bool EDataStream::read(T* value, quint64 size)
{
   if ( static_cast<quint64>(_file->read(reinterpret_cast<char*>(value),sizeof(T)*size))
        != sizeof(T)*size )
   {
      _status = Status::ReadFailed;
      return false;
   }
   return true;
}
