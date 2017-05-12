#include "dataobject.h"
#include "exception.h"
#include "abstractdatafactory.h"






Ace::DataObject::DataObject(const QString& path)
{
   _file.reset(new QFile(path));
   if ( !_file->open(QIODevice::ReadWrite) )
   {
      _status = CannotOpen;
      return;
   }
   _stream.reset(new EDataStream(_file.get()));
   if ( _file->size() <= _mininumFileSize )
   {
      return;
   }
   quint64 value {0};
   *_stream >> value;
   if ( !*_stream || value != _specialValue )
   {
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
   if ( dataType >= EAbstractDataFactory::getInstance().getCount()
        || extension != EAbstractDataFactory::getInstance().getFileExtension(dataType) )
   {
      _status = InvalidDataType;
      return;
   }
   _data = EAbstractDataFactory::getInstance().makeData(dataType);
   _headerOffset = _file->pos();
   _data->readData();
   _isNew = false;
}






Ace::DataObject::~DataObject()
{
   _data.reset();
   _stream.reset();
   _file.reset();
}






bool Ace::DataObject::seek(quint64 offset)
{
   if ( _status != Ok || _isNew )
   {
      return false;
   }
   return _file->seek(offset+_headerOffset);
}






bool Ace::DataObject::allocate(quint64 size)
{
   if ( _status != Ok || _isNew )
   {
      return false;
   }
   return _file->resize(_file->pos()+size);
}






Ace::DataObject::Status Ace::DataObject::getStatus() const
{
   return _status;
}






bool Ace::DataObject::clear(quint16 newType)
{
   if ( _status != Ok )
   {
      return false;
   }
   if ( newType >= EAbstractDataFactory::getInstance().getCount() )
   {
      _status = InvalidDataType;
      return false;
   }
   _data = EAbstractDataFactory::getInstance().makeData(newType);
   if ( !_file->seek(0) )
   {
      _status = CannotWrite;
      return false;
   }
   quint64 value = _specialValue;
   quint16 type = newType;
   *_stream << value << type << EAbstractDataFactory::getInstance().getFileExtension(newType);
   if ( !*_stream )
   {
      _status = CannotWrite;
      return false;
   }
   _headerOffset = _file->pos();
   _data->newData();
   _isNew = false;
   return true;
}






bool Ace::DataObject::isNew() const
{
   return _isNew;
}






EAbstractData& Ace::DataObject::data()
{
   if ( _status != Ok || _isNew )
   {
      EMAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Data Object Error"));
      e.out() << QObject::tr("Attempting to get data on failed/empty object with no data.");
      throw e;
   }
   return *_data;
}






Ace::DataObject::operator bool() const
{
   return _status == Ok;
}
