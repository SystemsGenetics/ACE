#include "dataobject.h"
#include "exception.h"
#include "abstractdatafactory.h"






DataObject::DataObject(const QString& path)
{
   _file.reset(new QFile(path));
   if ( !_file->open(QIODevice::ReadWrite) )
   {
      _status = CannotOpen;
      return;
   }
   _stream.reset(new DataStream(_file.get()));
   if ( _file->size() <= _mininumFileSize )
   {
      ;//make new file
      return;
   }
   quint64 value;
   *_stream >> value;
   if ( !*_stream )
   {
      ;//make new file
      return;
   }
   quint16 dataType;
   QString extension;
   *_stream >> dataType >> extension;
   if ( !*_stream )
   {
      _status = CorruptFile;
      return;
   }
   if ( dataType >= AbstractDataFactory::getInstance().getCount()
        || extension != AbstractDataFactory::getInstance().getFileExtension(dataType) )
   {
      _status = InvalidDataType;
      return;
   }
   _data = AbstractDataFactory::getInstance().makeData(dataType);
   _headerOffset = _file->pos();
   _data->readData();
   _isNew = false;
}






DataObject::~DataObject()
{
   _data.reset();
   _stream.reset();
   _file.reset();
}






bool DataObject::seek(quint64 offset)
{
}






bool DataObject::allocate(quint64 size)
{
}






DataObject::Status DataObject::getStatus() const
{
   return _status;
}






void DataObject::clear(const QString& newType)
{
}






bool DataObject::isNew() const
{
}






AbstractData& DataObject::data()
{
   if ( !_data )
   {
      MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Data Object Error"));
      e.out() << QObject::tr("Attempting to get data on failed object with no data.");
      throw e;
   }
   return *_data;
}






DataObject::operator bool() const
{
   return _status == Ok;
}
