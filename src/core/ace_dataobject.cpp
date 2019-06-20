#include "ace_dataobject.h"
#include "ace_datamanager.h"
#include "eabstractdata.h"
#include "edatastream.h"
#include "eabstractdatafactory.h"
#include "global.h"
#include "eexception.h"
#include "edebug.h"



namespace Ace
{






/*!
 * Creates a new data object from the file with the given path and optional
 * parent. If the path does not exist or it is corrupt then an exception is
 * thrown.
 *
 * @param path Path of the data object file to open.
 *
 * @param parent Parent for this data object.
 */
DataObject::DataObject(const QString& path, QObject* parent)
   :
   QObject(parent),
   _rawPath(path)
{
   // Add the debug header.
   EDEBUG_FUNC(this,path,parent);

   // Open the data object file, read in the header data, and call the read data
   // interface of this object's abstract data object.
   try
   {
      openObject(false);
      readHeader();
      _data->readData();
      seek(_data->dataEnd());
      stream() >> _userMeta;
      seek(0);
   }

   // If any exception occurs within this function then delete any allocated
   // resources and throw the exception again.
   catch (...)
   {
      delete _file;
      delete _stream;
      delete _data;
      throw;
   }
}






/*!
 * Creates a new data object file with the given type, the given file path, and
 * the given system metadata. Optionally a parent for this data object is given.
 * The file at the given path is overwritten. If the file truncation fails,
 * writing to it fails, or the given system metadata is not an object type then
 * an exception is thrown.
 *
 * @param path Path for this new data object's file that is overwritten.
 *
 * @param type Data object type for this newly created data object.
 *
 * @param system System metadata for this new data object. The metadata must be
 *               an object type or an exception is thrown.
 *
 * @param parent Parent for this data object.
 */
DataObject::DataObject(const QString& path, quint16 type, const EMetadata& system, QObject* parent)
   :
   QObject(parent),
   _rawPath(path),
   _type(type),
   _system(system)
{
   // Add the debug header.
   EDEBUG_FUNC(this,path,type,&system,parent);

   // Open the data object file, signal this new data object is overwriting the file,
   // write out the header data, and call the new data interface for this object's
   // abstract data object.
   try
   {
      openObject(true);
      DataManager::instance().newDataOpened(_path,this);
      writeHeader();
      _data->writeNewData();
   }

   // If any exception occurs within this function then delete any allocated
   // resources and throw the exception again.
   catch (...)
   {
      delete _file;
      delete _stream;
      delete _data;
      throw;
   }
}






/*!
 * Returns the raw path for this data object's file that was passed to it as a
 * constructor argument.
 *
 * @return Raw path that was passed to this data object's constructor.
 */
QString DataObject::rawPath() const
{
   EDEBUG_FUNC(this);
   return _rawPath;
}






/*!
 * Returns the path for this data object's file in absolute canonical terms.
 *
 * @return Canonical path to this data object's file.
 */
QString DataObject::path() const
{
   EDEBUG_FUNC(this);
   return _path;
}






/*!
 * Returns the file name for this data object.
 *
 * @return Name of this data object's file.
 */
QString DataObject::fileName() const
{
   EDEBUG_FUNC(this);
   return _fileName;
}






/*!
 * Returns the data type for this data object.
 *
 * @return Data object type.
 */
quint16 DataObject::type() const
{
   EDEBUG_FUNC(this);
   return _type;
}






/*!
 * Returns the current size of this data object in bytes. This does not include
 * the hidden header portion of the file of this data object.
 *
 * @return Size of this data object.
 */
qint64 DataObject::size() const
{
   EDEBUG_FUNC(this);
   return _file->size() - _headerOffset;
}






/*!
 * Returns the read only system metadata for this data object. The root metadata
 * object is always an object type.
 *
 * @return Root of system metadata for this data object.
 */
const EMetadata& DataObject::systemMeta() const
{
   EDEBUG_FUNC(this);
   return _system;
}






/*!
 * Returns the user metadata for this data object. The root metadata object is
 * always an object type.
 *
 * @return Root of user metadata for this data object.
 */
const EMetadata& DataObject::userMeta() const
{
   EDEBUG_FUNC(this);
   return _userMeta;
}






/*!
 * Seeks to the given index within this data object. This does not allow seeking
 * to the hidden header of this data object so seeking to 0 is one byte after
 * the header. If the given index is less than 0 or seeking fails then an
 * exception is thrown.
 *
 * @param index The index that the cursor position in this data object is set
 *              to.
 */
void DataObject::seek(qint64 index) const
{
   // Add the debug header.
   EDEBUG_FUNC(this,index);

   // If the given index is less than 0 then throw an exception, else to go the next
   // step.
   if ( index < 0 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Seeking index cannot be negative."));
      throw e;
   }

   // Call the qt file device seek function using the given index in addition to this
   // data object's header offset. If the seek call fails then throw an exception.
   if ( !_file->seek(index + _headerOffset) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("System Error"));
      e.setDetails(tr("Failed seeking in data object file: %1").arg(_file->errorString()));
      throw e;
   }
}






/*!
 * Returns a read only reference to the data stream for this data object.
 *
 * @return Read only reference to this objects data stream.
 */
const EDataStream& DataObject::stream() const
{
   EDEBUG_FUNC(this);
   return *_stream;
}






/*!
 * Allocates new space in this data object by the given number of bytes. The new
 * space requested is in addition to the current cursor position of this data
 * object. So if the cursor position is at the end of the data object then this
 * will grow the data object by the given amount. However if the cursor position
 * is at the beginning of the data object then this will only grow the data
 * object by the difference of the amount given and this data object's current
 * size. If the given size is less than 0 or resizing fails then an exception is
 * thrown.
 *
 * @param size The number of bytes to allocate after this data object's current
 *             cursor position.
 */
void DataObject::allocate(qint64 size)
{
   // Add the debug header.
   EDEBUG_FUNC(this,size);

   // If the given size is less than 0 then throw an exception, else go to the next
   // step.
   if ( size < 0 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Allocation size cannot be negative."));
      throw e;
   }

   // Resize this data object's file to the given size in addition to its current
   // cursor position. If the resize call fails then throw an exception.
   if ( !_file->resize(_file->pos() + size) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("System Error"));
      e.setDetails(tr("Failed resizing data object file: %1").arg(_file->errorString()));
      throw e;
   }
}






/*!
 * Returns a reference to the data stream for this data object.
 *
 * @return Reference to this objects data stream.
 */
EDataStream& DataObject::stream()
{
   EDEBUG_FUNC(this);
   return *_stream;
}






/*!
 * Returns a pointer to the abstract data object for this data object.
 *
 * @return Pointer to abstract data object.
 */
EAbstractData* DataObject::data()
{
   EDEBUG_FUNC(this);
   return _data;
}






/*!
 * Sets the user metadata for this data object with the given metadata object as
 * this object's new root. The given metadata value must be an object type.
 *
 * @param newRoot New root metadata value that this data object's user metadata
 *                is set to.
 */
void DataObject::setUserMeta(const EMetadata& newRoot)
{
   // Add the debug header.
   EDEBUG_FUNC(this,&newRoot);

   // If the given metadata is not an object type then throw on an exception, else go
   // to the next step.
   if ( !newRoot.isObject() )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("User metadata must be an object type."));
      throw e;
   }

   // Set this object's user metadata value to the new one given.
   _userMeta = newRoot;
}






/*!
 * Finalizes this new data object by writing out the user metadata to an empty
 * object if it has not already been written. If it has already been written or
 * this is not a new data object then this does nothing.
 */
void DataObject::finalize()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Write the user metadata to the end of this new data object file.
   seek(_data->dataEnd());
   stream() << _userMeta;
}






/*!
 * Reads in the given number of bytes from this data object at its current
 * cursor position and writes it to the given character pointer. If reading
 * failed then an exception is thrown.
 *
 * @param data Pointer to character array that is written to from the data read
 *             in this data object.
 *
 * @param size Number of bytes to read from this data object.
 */
void DataObject::read(char* data, qint64 size) const
{
   // Add the debug header.
   EDEBUG_FUNC(this,static_cast<void*>(data),size);

   // If this data object's file cursor position is less than the header offset and
   // this object's header has already been read then throw an exception, else go the
   // next step.
   if ( _headerRead && _file->pos() < _headerOffset )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logical Error"));
      e.setDetails(tr("Cannot read from header section of data object file."));
      throw e;
   }

   // Read the given number of bytes from this data object at the current cursor
   // position to the given character array. If reading failed then throw an
   // exception.
   if ( _file->read(data,size) != size )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("System Error"));
      e.setDetails(tr("Failed reading from data object file: %1").arg(_file->errorString()));
      throw e;
   }
}






/*!
 * Write out the given number of bytes to this data object at its current cursor
 * position from the given character pointer. If writing failed then an
 * exception is thrown.
 *
 * @param data Pointer to character array whose data is written to this data
 *             object.
 *
 * @param size The number of bytes to write to this data object from the given
 *             character array.
 */
void DataObject::write(const char* data, qint64 size)
{
   // Add the debug header.
   EDEBUG_FUNC(this,static_cast<const void*>(data),size);

   // If this data object's file cursor position is less than the header offset and
   // this object's header has already been read then throw an exception, else go the
   // next step.
   if ( _headerRead && _file->pos() < _headerOffset )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logical Error"));
      e.setDetails(tr("Cannot write to header section of data object file."));
      throw e;
   }

   // Write the given number of bytes to this data object at the current cursor
   // position from the given character array. If writing failed then throw an
   // exception.
   if ( _file->write(data,size) != size )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("System Error"));
      e.setDetails(tr("Failed writing to data object file: %1").arg(_file->errorString()));
      throw e;
   }
}






/*!
 * Called when a new data object has overwritten the given file path.
 *
 * @param canonicalPath Canonical path of the file a new data object is
 *                      overwriting.
 *
 * @param object Pointer to the new data object overwriting the given file path.
 */
void DataObject::dataOverwritten(const QString& canonicalPath, Ace::DataObject* object)
{
   // Add the debug header.
   EDEBUG_FUNC(this,canonicalPath,object);

   // If the given path matches this object's path and the given object pointer does
   // not match this object's pointer then emit the overwritten signal.
   if ( canonicalPath == _path && object != this )
   {
      emit overwritten();
   }
}






/*!
 * Opens this data object's file for reading or writing access and creates a new
 * data stream associated with this data object. If there is an error opening
 * the file then an exception is thrown.
 *
 * @param overwrite True if this is a new object file and being opened for
 *                  reading and writing or false if it is being opened for
 *                  reading.
 */
void DataObject::openObject(bool overwrite)
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Open this data object's file. If opening the file failed then throw an
   // exception, else go to the next step.
   _file = new QFile(_rawPath,this);
   if ( !_file->open(overwrite? QIODevice::ReadWrite : QIODevice::ReadOnly) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("System Error"));
      e.setDetails(tr("Cannot open file %1; %2").arg(_rawPath).arg(_file->errorString()));
      throw e;
   }

   // Create a data stream object and assign it to this data object.
   _stream = new EDataStream(this);

   // Connect data manager signals with this object and set this data object's file
   // name and canonical file path.
   connect(&DataManager::instance()
           ,&Ace::DataManager::dataOverwritten
           ,this
           ,&DataObject::dataOverwritten);
   QFileInfo info(_rawPath);
   _path = info.canonicalPath();
   _fileName = info.fileName();
}






/*!
 * Reads in the header portion of this data object's file, creating the abstract
 * data interface as well. This assumes the file already contains an existing
 * ACE data object. If reading in the file as a data object fails then an
 * exception is thrown.
 */
void DataObject::readHeader()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Read in this data object's special value, type, name, extension, and system
   // metadata of this data object's file. If the special value is incorrect then
   // throw an exception, else go to the next step.
   qint64 value;
   QString name;
   QString extension;
   stream() >> value >> _type >> name >> extension >> _system;
   if ( value != _specialValue )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Read Error"));
      e.setDetails(tr("Failed reading in file %1: it is not a data object file.").arg(_rawPath));
      throw e;
   }

   // Create a new abstract data object for this data object and set this data
   // object's header offset to the current cursor position of this data object's
   // file.
   makeData(name,extension);
   _headerOffset = _file->pos();

   // Set this data object's header as read.
   _headerRead = true;
}






/*!
 * Writes out the header portion of this data object to its file, creating a new
 * abstract data object as well. This will truncate the existing file and
 * overwrite any existing data. If truncation fails or the system metadata is
 * not an object type then an exception is thrown.
 */
void DataObject::writeHeader()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // If this new data object's system metadata is not an object type then throw an
   // exception, else go to the next step.
   if ( !_system.isObject() )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("System metadata must be an object type."));
      throw e;
   }

   // Make a new abstract data object for this data object, providing the correct
   // name and extension for it.
   EAbstractDataFactory& factory {EAbstractDataFactory::instance()};
   makeData(factory.name(_type),factory.fileExtension(_type));

   // Truncate this data object's file to 0. If the truncation failed then throw an
   // exception, else go to the next step.
   if ( !_file->resize(0) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("System Error"));
      e.setDetails(tr("Failed truncating file %1: %2").arg(_rawPath).arg(_file->errorString()));
      throw e;
   }

   // Write out this data object's special value, type, name, extension, and system
   // metadata to this data object's file.
   stream() << _specialValue
            << _type << factory.name(_type)
            << factory.fileExtension(_type)
            << _system;

   // Set this data object's header offset to the current cursor position of this
   // data object's file.
   _headerOffset = _file->pos();

   // Set this data object's header as read and its user metadata as not written.
   _headerRead = true;
}






/*!
 * Makes a new abstract data object for this data object using the type
 * specified in this data object and checking its validity with the given name
 * and extension. If this data object's type is out of range, the given name
 * does not match, or the given extension does not mach then an exception is
 * thrown.
 *
 *
 */
void DataObject::makeData(const QString& name, const QString& extension)
{
   // Add the debug header.
   EDEBUG_FUNC(this,name,extension);

   // If the given name or extension does not match would they should be for the
   // given data type then throw an exception, else create a new abstract data object
   // setting its parent to this data object.
   EAbstractDataFactory& factory {EAbstractDataFactory::instance()};
   if ( _type >= factory.size()
        || name != factory.name(_type)
        || extension != factory.fileExtension(_type) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Data type given for creation of new data object is invalid."));
      throw e;
   }
   _data = factory.make(_type).release();
   _data->setParent(this);
}

}
