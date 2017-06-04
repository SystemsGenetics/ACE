#include "dataobject.h"
#include "exception.h"
#include "abstractdatafactory.h"






Ace::DataObject::DataObject(const QString& path)
{
   try
   {
      _file.reset(new QFile(path));
      if ( !_file->open(QIODevice::ReadWrite) )
      {
         E_MAKE_EXCEPTION(e);
         e.setLevel(EException::Critical);
         e.setType(CannotOpen);
         e.setTitle(tr("Open Data Object"));
         e.setDetails(tr("Cannot open file %1.").arg(path));
         throw e;
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
      QString name;
      QString extension;
      *_stream >> dataType >> name >> extension;
      if ( !*_stream )
      {
         E_MAKE_EXCEPTION(e);
         e.setLevel(EException::Notice);
         e.setType(CorruptFile);
         e.setTitle(tr("Open Data Object"));
         e.setDetails(tr("Cannot load file %1 because it's header is corrupt.").arg(path));
         throw e;
      }
      EAbstractDataFactory& factory {EAbstractDataFactory::getInstance()};
      if ( dataType >= factory.getCount() || name != factory.getName(dataType)
           || extension != factory.getFileExtension(dataType) )
      {
         E_MAKE_EXCEPTION(e);
         e.setLevel(EException::Notice);
         e.setType(InvalidDataType);
         e.setTitle(tr("Open Data Object"));
         e.setDetails(tr("Cannot load file %1 because it's header specifies an invalid data type.")
                      .arg(path));
         throw e;
      }
      _data = EAbstractDataFactory::getInstance().make(dataType);
      _headerOffset = _file->pos();
      _data->initialize(this,_stream.get());
      _isNew = false;
   }
   catch (EException e)
   {
      if ( e.getLevel() == EException::Critical )
      {
         _invalid = true;
      }
      throw;
   }
}






Ace::DataObject::~DataObject() noexcept
{
   _data.reset();
   _stream.reset();
   _file.reset();
}






bool Ace::DataObject::seek(quint64 offset) noexcept
{
   if ( _invalid || _isNew )
   {
      return false;
   }
   return _file->seek(offset+_headerOffset);
}






bool Ace::DataObject::allocate(quint64 size) noexcept
{
   if ( _invalid || _isNew )
   {
      return false;
   }
   return _file->resize(_file->pos()+size);
}






void Ace::DataObject::clear(quint16 newType)
{
   if ( _invalid )
   {
      return;
   }
   try
   {
      EAbstractDataFactory& factory {EAbstractDataFactory::getInstance()};
      if ( newType >= factory.getCount() )
      {
         E_MAKE_EXCEPTION(e);
         e.setLevel(EException::Notice);
         e.setType(InvalidDataType);
         e.setTitle(tr("Clear Data Object"));
         e.setDetails(tr("Cannot initialize data object with unknown type %1.").arg(newType));
         throw e;
      }
      if ( _data )
      {
         emit cleared();
      }
      _data = factory.make(newType);
      if ( !_file->seek(0) )
      {
         E_MAKE_EXCEPTION(e);
         e.setLevel(EException::Critical);
         e.setType(CannotWrite);
         e.setTitle(tr("Data Object I/O"));
         e.setDetails(tr("Cannot write to data object file."));
         throw e;
      }
      quint64 value = _specialValue;
      quint16 type = newType;
      *_stream << value << type << factory.getName(newType) << factory.getFileExtension(newType);
      if ( !*_stream )
      {
         E_MAKE_EXCEPTION(e);
         e.setLevel(EException::Critical);
         e.setType(CannotWrite);
         e.setTitle(tr("Data Object I/O"));
         e.setDetails(tr("Cannot write to data object file."));
         throw e;
      }
      _headerOffset = _file->pos();
      _data->initialize(this,_stream.get());
      _data->newData();
      _isNew = false;
   }
   catch (EException e)
   {
      if ( e.getLevel() == EException::Critical )
      {
         _invalid = true;
      }
      throw;
   }
}






bool Ace::DataObject::isNew() const noexcept
{
   return _isNew;
}






EAbstractData& Ace::DataObject::data()
{
   if ( _invalid || _isNew )
   {
      E_MAKE_EXCEPTION(e);
      e.setLevel(EException::Critical);
      e.setType(NullReference);
      e.setTitle(QObject::tr("Data Object Reference"));
      e.setDetails(QObject::tr("Attempting to get data on failed/empty object with no data."));
      throw e;
   }
   return *_data;
}






Ace::DataObject::operator bool() const noexcept
{
   return !_invalid;
}
