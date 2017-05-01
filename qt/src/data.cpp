#include "data.h"






Data::~Data()
{
   delete _file;
   delete _stream;
}






bool Data::open(const QString& path, QIODevice::OpenModeFlag flags)
{
   delete _file;
   delete _stream;
   _stream = nullptr;
   _file = new QFile(path);
   if ( _file->open(flags) )
   {
      _good = true;
   }
   else
   {
      _good = false;
   }
   return _good;
}






quint64 Data::allocate(quint64 size)
{

}






quint64 Data::size(Data::MemoryType type) const
{

}






bool Data::isGood() const
{

}






bool Data::seek(quint64 location) const
{

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
