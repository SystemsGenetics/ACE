#include <QtEndian>
#include <QPixmap>

#include "datastream.h"



using namespace std;






EDataStream::EDataStream(QFile *file):
   _file(file)
{}






EDataStream& EDataStream::operator<<(qint8 value)
{
   if ( *this )
   {
      write(value);
   }
   return *this;
}






EDataStream& EDataStream::operator<<(quint8 value)
{
   if ( *this )
   {
      write(value);
   }
   return *this;
}






EDataStream& EDataStream::operator<<(qint16 value)
{
   if ( *this )
   {
      value = qToBigEndian(value);
      write(value);
   }
   return *this;
}






EDataStream& EDataStream::operator<<(quint16 value)
{
   if ( *this )
   {
      value = qToBigEndian(value);
      write(value);
   }
   return *this;
}






EDataStream& EDataStream::operator<<(qint32 value)
{
   if ( *this )
   {
      value = qToBigEndian(value);
      write(value);
   }
   return *this;
}






EDataStream& EDataStream::operator<<(quint32 value)
{
   if ( *this )
   {
      value = qToBigEndian(value);
      write(value);
   }
   return *this;
}






EDataStream& EDataStream::operator<<(qint64 value)
{
   if ( *this )
   {
      value = qToBigEndian(value);
      write(value);
   }
   return *this;
}






EDataStream& EDataStream::operator<<(quint64 value)
{
   if ( *this )
   {
      value = qToBigEndian(value);
      write(value);
   }
   return *this;
}






EDataStream& EDataStream::operator<<(float value)
{
   if ( *this )
   {
      write(value);
   }
   return *this;
}






EDataStream& EDataStream::operator<<(double value)
{
   if ( *this )
   {
      write(value);
   }
   return *this;
}






EDataStream& EDataStream::operator<<(const QString& value)
{
   if ( *this )
   {
      if ( value.size() > _maxStringSize )
      {
         E_MAKE_EXCEPTION(e);
         e.setLevel(EException::Critical);
         e.setType(StringTooBig);
         e.setTitle(QObject::tr("Data Stream Write"));
         e.setDetails(QObject::tr("Could not write string because it is too large."));
         setException(e);
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
   if ( *this )
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
   if ( *this )
   {
      read(&value);
   }
   return *this;
}






EDataStream& EDataStream::operator>>(quint8& value)
{
   value = 0;
   if ( *this )
   {
      read(&value);
   }
   return *this;
}






EDataStream& EDataStream::operator>>(qint16& value)
{
   value = 0;
   if ( *this )
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
   if ( *this )
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
   if ( *this )
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
   if ( *this )
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
   if ( *this )
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
   if ( *this )
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
   if ( *this )
   {
      read(&value);
   }
   return *this;
}






EDataStream& EDataStream::operator>>(double& value)
{
   value = 0.0;
   if ( *this )
   {
      read(&value);
   }
   return *this;
}






EDataStream& EDataStream::operator>>(QString& value)
{
   value.clear();
   if ( *this )
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
            E_MAKE_EXCEPTION(e);
            e.setLevel(EException::Critical);
            e.setType(CorruptData);
            e.setTitle(QObject::tr("Data Stream Read"));
            e.setDetails(QObject::tr("Could not read string because data is corrupt"));
            setException(e);
         }
      }
   }
   return *this;
}






EDataStream& EDataStream::operator>>(QPixmap& value)
{
   value = QPixmap();
   if ( *this )
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
                     E_MAKE_EXCEPTION(e);
                     e.setLevel(EException::Critical);
                     e.setType(CorruptData);
                     e.setTitle(QObject::tr("Data Stream Read"));
                     e.setDetails(QObject::tr("Could not read image because data is corrupt"));
                     setException(e);
                  }
               }
            }
         }
         else
         {
            E_MAKE_EXCEPTION(e);
            e.setLevel(EException::Critical);
            e.setType(CorruptData);
            e.setTitle(QObject::tr("Data Stream Read"));
            e.setDetails(QObject::tr("Could not read image because data is corrupt"));
            setException(e);
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
      E_MAKE_EXCEPTION(e);
      e.setLevel(EException::Critical);
      e.setType(WriteFailed);
      e.setTitle(QObject::tr("Data Stream Write"));
      e.setDetails(QObject::tr("Failed writing to file: %1").arg(_file->errorString()));
      setException(e);
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
      E_MAKE_EXCEPTION(e);
      e.setLevel(EException::Critical);
      e.setType(ReadFailed);
      e.setTitle(QObject::tr("Data Stream Read"));
      e.setDetails(QObject::tr("Failed reading from file: %1").arg(_file->errorString()));
      setException(e);
      return false;
   }
   return true;
}
