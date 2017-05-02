#include <QtEndian>

#include "data.h"



using namespace std;






Data::~Data()
{
   delete _file;
}






unique_ptr<Data> Data::open(const QString& path)
{
   QFile* file = new QFile(path);
   if ( !file->open(QIODevice::ReadWrite) )
   {
      return nullptr;
   }
   quint64 usedSize {0};
   if ( !file->seek(0) )
   {
      return nullptr;
   }
   if ( file->size() >= sizeof(quint64) )
   {
      if ( file->readData(reinterpret_cast<char*>(&usedSize),sizeof(quint64)) != sizeof(quint64) )
      {
         return nullptr;
      }
      usedSize = qFromBigEndian(usedSize);
   }
   else
   {
      quint64 a = qToBigEndian(usedSize);
      if ( file->writeData(reinterpret_cast<char*>(&a),sizeof(quint64)) != sizeof(quint64) )
      {
         return nullptr;
      }
   }
   if ( !file->seek(sizeof(quint64)) )
   {
      return nullptr;
   }
   return unique_ptr<Data>(new Data(file,usedSize));
}






quint64 Data::allocate(quint64 size)
{
   if ( !_file )
   {
      ;//uh oh!
   }
   quint64 pointer = _file->size();
   if ( size > (_file->size()-_usedSize) )
   {
      if ( !_file->resize(size-_file->size()+_usedSize) )
      {
         return 0;
      }
   }
   return pointer;
}






quint64 Data::size(Data::MemoryType type) const
{
   if ( !_file )
   {
      ;//uh oh!
   }
   switch (type)
   {
   case MemoryType::Used:
      return _usedSize;
   case MemoryType::Free:
      return _file->size()-_usedSize;
   case MemoryType::Total:
      return _file->size();
   }
}






bool Data::seek(quint64 location)
{
   if ( !_file )
   {
      ;//uh oh!
   }
   return _file->seek(location);
}






QDataStream& Data::stream()
{
   if ( !_file )
   {
      ;//uh oh!
   }
   if ( !_stream )
   {
      _stream = new QDataStream(_file);
   }
   return *_stream;
}






void Data::clear()
{
}
