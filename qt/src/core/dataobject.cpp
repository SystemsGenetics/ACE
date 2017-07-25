#include "dataobject.h"
#include "exception.h"
#include "abstractdatafactory.h"






Ace::DataObject::DataObject(const QString& path):
   _metaRoot(EMetadata::Object)
{
   try
   {
      // open file from given path and make sure it opened for read/write
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

      // create a new data stream from the open file
      _stream.reset(new EDataStream(_file.get()));

      // if file size is less than minimum size return as new object
      if ( _file->size() <= _mininumFileSize )
      {
         return;
      }

      // read special identificaiton value
      quint64 value {0};
      *_stream >> value;

      // if read failed for value is wrong return as new object
      if ( !*_stream || value != _specialValue )
      {
         return;
      }

      // read data type, name, and extension
      QString name;
      QString extension;
      *_stream >> _type >> name >> extension;

      // make sure read was successful
      if ( !*_stream )
      {
         E_MAKE_EXCEPTION(e);
         e.setLevel(EException::Notice);
         e.setType(CorruptFile);
         e.setTitle(tr("Open Data Object"));
         e.setDetails(tr("Cannot load file %1 because it's header is corrupt.").arg(path));
         throw e;
      }

      // make sure data type in header is valid
      EAbstractDataFactory& factory {EAbstractDataFactory::getInstance()};
      if ( _type >= factory.getCount() || name != factory.getName(_type)
           || extension != factory.getFileExtension(_type) )
      {
         E_MAKE_EXCEPTION(e);
         e.setLevel(EException::Notice);
         e.setType(InvalidDataType);
         e.setTitle(tr("Open Data Object"));
         e.setDetails(tr("Cannot load file %1 because it's header specifies an invalid data type.")
                      .arg(path));
         throw e;
      }

      // create new abstract data object of type given in header and get header offset
      _data = EAbstractDataFactory::getInstance().make(_type);
      _headerOffset = _file->pos();

      // call data initialize function and get data offset
      _data->initialize(this,_stream.get());
      _dataOffset = _data->getDataEnd();

      // seek to data offset and read in metadata
      seek(_dataOffset);
      *_stream >> _metaRoot;

      // make sure reading ot metadata worked
      if ( !*_stream )
      {
         E_MAKE_EXCEPTION(e);
         e.setLevel(EException::Notice);
         e.setType(CorruptFile);
         e.setTitle(tr("Open Data Object"));
         e.setDetails(tr("Cannot load file %1 because it's metadata is corrupt.").arg(path));
         throw e;
      }

      // set data object to not new
      _isNew = false;
   }
   catch (EException e)
   {
      // clear any allocated objects
      _file.reset();
      _stream.reset();
      _data.reset();
      throw;
   }
}






Ace::DataObject::~DataObject() noexcept
{
   // order is important data must be cleared before stream, stream before file
   _data.reset();
   _stream.reset();
   _file.reset();
}






bool Ace::DataObject::seek(quint64 offset) noexcept
{
   // make sure data object is not invalid or new
   if ( _invalid || _isNew || offset >= (_file->size()-_headerOffset) )
   {
      return false;
   }

   // call qt seek function and return it's return
   return _file->seek(offset+_headerOffset);
}






bool Ace::DataObject::allocate(quint64 size) noexcept
{
   // make sure data object is not invalid or new
   if ( _invalid || _isNew )
   {
      return false;
   }

   // call qt resize function with additional space given to current header position, return it's
   // return
   return _file->resize(_file->pos()+size);
}






void Ace::DataObject::clear(quint16 newType)
{
   // if data object is invalid do nothing
   if ( _invalid )
   {
      return;
   }
   try
   {
      // make sure type given is valid
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

      // if data object already had data emit cleared signal
      if ( _data )
      {
         emit cleared();
      }

      // make new abstract data with given type
      _data = factory.make(newType);

      // seek to beginning of file
      if ( !_file->seek(0) )
      {
         E_MAKE_EXCEPTION(e);
         e.setLevel(EException::Critical);
         e.setType(CannotWrite);
         e.setTitle(tr("Data Object I/O"));
         e.setDetails(tr("Cannot write to data object file."));
         throw e;
      }

      // write new header with information about new data type
      quint64 value = _specialValue;
      _type = newType;
      *_stream << value << _type << factory.getName(newType) << factory.getFileExtension(newType);
      if ( !*_stream )
      {
         E_MAKE_EXCEPTION(e);
         e.setLevel(EException::Critical);
         e.setType(CannotWrite);
         e.setTitle(tr("Data Object I/O"));
         e.setDetails(tr("Cannot write to data object file."));
         throw e;
      }

      // get new header offset in file
      _headerOffset = _file->pos();

      // initialize new abstract data object, call its new data function, and mark this data object
      // as not new
      _data->initialize(this,_stream.get());
      _data->newData();
      _isNew = false;
   }
   catch (EException e)
   {
      // if any critical exception occurs while clearing make data object invalid
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






quint64 Ace::DataObject::size() const noexcept
{
   // make sure object is valid and not new
   if ( _invalid || _isNew )
   {
      return -1;
   }

   // return size of file
   return (quint64)_file->size()-_headerOffset;
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






void Ace::DataObject::writeMeta()
{
   // write out meta information after data
   seek(_dataOffset);
   *_stream << _metaRoot;

   // make sure writing was successful
   if ( !*_stream )
   {
      E_MAKE_EXCEPTION(e);
      e.setLevel(EException::Critical);
      e.setType(CannotWrite);
      e.setTitle(tr("Data Object I/O"));
      e.setDetails(tr("Failed writing metadata of object."));
      throw e;
   }
}






Ace::DataObject::operator bool() const noexcept
{
   return !_invalid;
}
