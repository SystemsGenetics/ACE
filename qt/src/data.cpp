#include <QtEndian>
#include <QPixmap>

#include "data.h"



using namespace std;






Data::~Data()
{
   delete _file;
}






unique_ptr<Data> Data::open(const QString& path)
{
   unique_ptr<QFile> file(new QFile(path));
   if ( !file->open(QIODevice::ReadWrite) )
   {
      return nullptr;
   }
   quint64 usedSize {0};
   if ( !file->seek(0) )
   {
      return nullptr;
   }
   if ( static_cast<quint64>(file->size()) >= sizeof(quint64) )
   {
      if ( file->read(reinterpret_cast<char*>(&usedSize),sizeof(quint64)) != sizeof(quint64) )
      {
         return nullptr;
      }
      usedSize = qFromBigEndian(usedSize);
   }
   else
   {
      quint64 a = qToBigEndian(usedSize);
      if ( file->write(reinterpret_cast<char*>(&a),sizeof(quint64)) != sizeof(quint64) )
      {
         return nullptr;
      }
   }
   if ( !file->seek(sizeof(quint64)) )
   {
      return nullptr;
   }
   return unique_ptr<Data>(new Data(file.release(),usedSize));
}






quint64 Data::allocate(quint64 size)
{
   if ( size > getFreeSize() )
   {
      if ( !_file->resize(size+getUsedSize()+sizeof(quint64)) )
      {
         _status = Status::AllocateFailed;
         return 0;
      }
   }
   quint64 pointer = _usedSize;
   if ( !_file->seek(0) )
   {
      _status = Status::AllocateFailed;
      return 0;
   }
   quint64 a = qToBigEndian(_usedSize+size);
   if ( _file->write(reinterpret_cast<char*>(&a),sizeof(quint64)) != sizeof(quint64) )
   {
      _status = Status::AllocateFailed;
      return 0;
   }
   _usedSize += size;
   return pointer;
}






quint64 Data::getUsedSize() const
{
   return _usedSize;
}






quint64 Data::getFreeSize() const
{
   return _file->size()-_usedSize-sizeof(quint64);
}






quint64 Data::getTotalSize() const
{
   return _file->size()-sizeof(quint64);
}






Data::Status Data::getStatus() const
{
   return _status;
}






bool Data::seek(quint64 location)
{
   return _file->seek(location+sizeof(quint64));
}






void Data::clear()
{
   int a = qToBigEndian(0);
   if ( _file->write(reinterpret_cast<char*>(&a),sizeof(quint64)) != sizeof(quint64) )
   {
      _status = Status::ClearFailed;
      return;
   }
   _usedSize = 0;
}






Data& Data::operator<<(qint8 value)
{
   write(value);
   return *this;
}






Data& Data::operator<<(quint8 value)
{
   write(value);
   return *this;
}






Data& Data::operator<<(qint16 value)
{
   value = qToBigEndian(value);
   write(value);
   return *this;
}






Data& Data::operator<<(quint16 value)
{
   value = qToBigEndian(value);
   write(value);
   return *this;
}






Data& Data::operator<<(qint32 value)
{
   value = qToBigEndian(value);
   write(value);
   return *this;
}






Data& Data::operator<<(quint32 value)
{
   value = qToBigEndian(value);
   write(value);
   return *this;
}






Data& Data::operator<<(qint64 value)
{
   value = qToBigEndian(value);
   write(value);
   return *this;
}






Data& Data::operator<<(quint64 value)
{
   value = qToBigEndian(value);
   write(value);
   return *this;
}






Data& Data::operator<<(float value)
{
   write(value);
   return *this;
}






Data& Data::operator<<(double value)
{
   write(value);
   return *this;
}






Data& Data::operator<<(const QString& value)
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






Data &Data::operator<<(const QPixmap &value)
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






Data& Data::operator>>(qint8& value)
{
   read(&value);
   return *this;
}






Data& Data::operator>>(quint8& value)
{
   read(&value);
   return *this;
}






Data& Data::operator>>(qint16& value)
{
   if ( read(&value) )
   {
      value = qFromBigEndian(value);
   }
   return *this;
}






Data& Data::operator>>(quint16& value)
{
   if ( read(&value) )
   {
      value = qFromBigEndian(value);
   }
   return *this;
}






Data& Data::operator>>(qint32& value)
{
   if ( read(&value) )
   {
      value = qFromBigEndian(value);
   }
   return *this;
}






Data& Data::operator>>(quint32& value)
{
   if ( read(&value) )
   {
      value = qFromBigEndian(value);
   }
   return *this;
}






Data& Data::operator>>(qint64& value)
{
   if ( read(&value) )
   {
      value = qFromBigEndian(value);
   }
   return *this;
}






Data& Data::operator>>(quint64& value)
{
   if ( read(&value) )
   {
      value = qFromBigEndian(value);
   }
   return *this;
}






Data& Data::operator>>(float& value)
{
   read(&value);
   return *this;
}






Data& Data::operator>>(double& value)
{
   read(&value);
   return *this;
}






Data& Data::operator>>(QString& value)
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
            QChar* buffer = new QChar[size+1];
            if ( read(buffer,size) )
            {
               buffer[size] = QChar('\0');
               value = QString(buffer,size);
            }
            delete[] buffer;
         }
      }
      else
      {
         _status = Status::CorruptData;
      }
   }
   return *this;
}






Data &Data::operator>>(QPixmap &value)
{
   quint8 type {0};
   if ( read(&type) )
   {
      if ( type == Pixmap )
      {
         quint16 size;
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






Data::Data(QFile *file, quint64 usedSize):
   _file(file),
   _usedSize(usedSize)
{}






template<class T>
bool Data::write(T value, quint64 size)
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
bool Data::read(T* value, quint64 size)
{
   if ( static_cast<quint64>(_file->read(reinterpret_cast<char*>(value),sizeof(T)*size))
        != sizeof(T)*size )
   {
      _status = Status::ReadFailed;
      return false;
   }
   return true;
}
