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
}






bool Ace::DataObject::allocate(quint64 size)
{
}






Ace::DataObject::Status Ace::DataObject::getStatus() const
{
   return _status;
}






void Ace::DataObject::clear(const QString& newType)
{
}






bool Ace::DataObject::isNew() const
{
}






EAbstractData& Ace::DataObject::data()
{
   if ( !_data )
   {
      EMAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Data Object Error"));
      e.out() << QObject::tr("Attempting to get data on failed object with no data.");
      throw e;
   }
   return *_data;
}






Ace::DataObject::operator bool() const
{
   return _status == Ok;
}
