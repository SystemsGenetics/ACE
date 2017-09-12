#include "dataobject.h"
#include "exception.h"
#include "abstractdatafactory.h"






Ace::DataObject::DataObject(const QString& path):
   _metaRoot(EMetadata::Object)
   ,_path(path)
{
   try
   {
      // open file from given path and make sure it opened for read/write
      _file.reset(new QFile(path));
      if ( !_file->open(QIODevice::ReadWrite) )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(tr("Open Data Object"));
         e.setDetails(tr("Cannot open file %1.").arg(path));
         throw e;
      }

      // create a new data stream from the open file
      _stream.reset(new EDataStream(_file.get()));
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






Ace::DataObject::~DataObject()
{
   // order is important data must be cleared before stream, stream before file
   _data.reset();
   _stream.reset();
   _file.reset();
}






void Ace::DataObject::open()
{
   try
   {
      // seek to beginning of file
      if ( !_file->seek(0) )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(tr("Open Data Object"));
         e.setDetails(tr("Cannot seek to beginning of data file."));
         throw e;
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
         e.setTitle(tr("Open Data Object"));
         e.setDetails(tr("Cannot load file because it's header is corrupt."));
         throw e;
      }

      // make sure data type in header is valid
      EAbstractDataFactory& factory {EAbstractDataFactory::getInstance()};
      if ( _type >= factory.getCount() || name != factory.getName(_type)
           || extension != factory.getFileExtension(_type) )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(tr("Open Data Object"));
         e.setDetails(tr("Cannot load file because it's header specifies an invalid data type."));
         throw e;
      }

      // create new abstract data object of type given in header and get header offset
      _data = EAbstractDataFactory::getInstance().make(_type);
      _headerOffset = _file->pos();

      // call data initialize function, read data function, and get data offset
      _data->initialize(this,_stream.get());
      _data->readData();

      // seek to data offset and read in metadata
      seek(_data->getDataEnd());
      *_stream >> _metaRoot;
      _metaModel.setRoot(&_metaRoot);

      // make sure reading ot metadata worked
      if ( !*_stream )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(tr("Open Data Object"));
         e.setDetails(tr("Cannot load file because it's metadata is corrupt."));
         throw e;
      }

      // set data object to not new
      _isNew = false;
   }
   catch (...)
   {
      // clear any allocated data object and set to new
      _data.reset();
      _isNew = true;
      throw;
   }
}






bool Ace::DataObject::seek(qint64 offset) const
{
   // make sure data object is not invalid
   if ( _invalid || offset < 0 || offset > (_file->size()-_headerOffset) )
   {
      return false;
   }

   // call qt seek function and return it's return
   return _file->seek(offset+_headerOffset);
}






bool Ace::DataObject::allocate(qint64 size)
{
   // make sure data object is not invalid or new
   if ( _invalid || size < 0 )
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
      _metaModel.setRoot(nullptr);
      // make sure type given is valid
      EAbstractDataFactory& factory {EAbstractDataFactory::getInstance()};
      if ( newType >= factory.getCount() )
      {
         E_MAKE_EXCEPTION(e);
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

      _metaRoot.setType(EMetadata::Object);
      _metaModel.setRoot(&_metaRoot);
   }
   catch (...)
   {
      _invalid = true;
      throw;
   }
}






bool Ace::DataObject::isNew() const
{
   return _isNew;
}






qint64 Ace::DataObject::size() const
{
   // make sure object is valid and not new
   if ( _invalid || _isNew )
   {
      return -1;
   }

   // return size of file
   return (qint64)_file->size()-_headerOffset;
}






EAbstractData& Ace::DataObject::data()
{
   // make sure there is abstract data object to return
   if ( _invalid || _isNew )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Data Object Reference"));
      e.setDetails(tr("Attempting to get data on failed/empty object with no data."));
      throw e;
   }

   // return reference
   return *_data;
}






void Ace::DataObject::writeMeta()
{
   // write out meta information after data
   seek(_data->getDataEnd());
   *_stream << _metaRoot;

   // make sure writing was successful
   if ( !*_stream )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Data Object I/O"));
      e.setDetails(tr("Failed writing metadata of object."));
      throw e;
   }
}






void Ace::DataObject::reloadMeta()
{
   _metaModel.setRoot(nullptr);
   seek(_data->getDataEnd());
   *_stream >> _metaRoot;
   _metaModel.setRoot(&_metaRoot);
}






Ace::DataObject::operator bool() const
{
   return !_invalid;
}
