#include <QtEndian>
#include <QPixmap>

#include "datastream.h"



using namespace std;






DataStream::DataStream(QFile *file):
   _file(file)
{}






DataStream::~DataStream()
{
   delete _file;
}






DataStream::Status DataStream::getStatus() const
{
   return _status;
}






DataStream& DataStream::operator<<(qint8 value)
{
   write(value);
   return *this;
}






DataStream& DataStream::operator<<(quint8 value)
{
   write(value);
   return *this;
}






DataStream& DataStream::operator<<(qint16 value)
{
   value = qToBigEndian(value);
   write(value);
   return *this;
}






DataStream& DataStream::operator<<(quint16 value)
{
   value = qToBigEndian(value);
   write(value);
   return *this;
}






DataStream& DataStream::operator<<(qint32 value)
{
   value = qToBigEndian(value);
   write(value);
   return *this;
}






DataStream& DataStream::operator<<(quint32 value)
{
   value = qToBigEndian(value);
   write(value);
   return *this;
}






DataStream& DataStream::operator<<(qint64 value)
{
   value = qToBigEndian(value);
   write(value);
   return *this;
}






DataStream& DataStream::operator<<(quint64 value)
{
   value = qToBigEndian(value);
   write(value);
   return *this;
}






DataStream& DataStream::operator<<(float value)
{
   write(value);
   return *this;
}






DataStream& DataStream::operator<<(double value)
{
   write(value);
   return *this;
}






DataStream& DataStream::operator<<(const QString& value)
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
   return *this;
}






DataStream &DataStream::operator<<(const QPixmap &value)
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
   return *this;
}






DataStream& DataStream::operator>>(qint8& value)
{
   read(&value);
   return *this;
}






DataStream& DataStream::operator>>(quint8& value)
{
   read(&value);
   return *this;
}






DataStream& DataStream::operator>>(qint16& value)
{
   if ( read(&value) )
   {
      value = qFromBigEndian(value);
   }
   return *this;
}






DataStream& DataStream::operator>>(quint16& value)
{
   if ( read(&value) )
   {
      value = qFromBigEndian(value);
   }
   return *this;
}






DataStream& DataStream::operator>>(qint32& value)
{
   if ( read(&value) )
   {
      value = qFromBigEndian(value);
   }
   return *this;
}






DataStream& DataStream::operator>>(quint32& value)
{
   if ( read(&value) )
   {
      value = qFromBigEndian(value);
   }
   return *this;
}






DataStream& DataStream::operator>>(qint64& value)
{
   if ( read(&value) )
   {
      value = qFromBigEndian(value);
   }
   return *this;
}






DataStream& DataStream::operator>>(quint64& value)
{
   if ( read(&value) )
   {
      value = qFromBigEndian(value);
   }
   return *this;
}






DataStream& DataStream::operator>>(float& value)
{
   read(&value);
   return *this;
}






DataStream& DataStream::operator>>(double& value)
{
   read(&value);
   return *this;
}






DataStream& DataStream::operator>>(QString& value)
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
   return *this;
}






DataStream& DataStream::operator>>(QPixmap &value)
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
   return *this;
}






template<class T>
bool DataStream::write(T value, quint64 size)
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
bool DataStream::read(T* value, quint64 size)
{
   if ( static_cast<quint64>(_file->read(reinterpret_cast<char*>(value),sizeof(T)*size))
        != sizeof(T)*size )
   {
      _status = Status::ReadFailed;
      return false;
   }
   return true;
}
