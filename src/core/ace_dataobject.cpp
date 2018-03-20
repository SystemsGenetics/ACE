#include "ace_dataobject.h"
#include "exception.h"
#include "abstractdatafactory.h"
#include "datastream.h"
#include "common.h"



using namespace Ace;
//






/*!
 * Creates a new data object from the file with the given path and optional 
 * parent. If the path does not exist or it is corrupt then an exception is 
 * thrown. 
 *
 * @param path Path of the data object file to open. 
 *
 * @param parent Parent for this data object. 
 */
DataObject::DataObject(const QString& path, QObject* parent):
   QObject(parent),
   _path(path)
{
   try
   {
      openObject();
      readHeader();
      _data->readData();
   }
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
 * The file at the given path is overwritten. 
 *
 * @param path Path for this new data object's file that is overwritten. 
 *
 * @param type Data object type for this newly created data object. 
 *
 * @param system  
 *
 * @param parent Parent for this data object. 
 */
DataObject::DataObject(const QString& path, quint16 type, const EMetadata& system, QObject* parent):
   QObject(parent),
   _type(type),
   _path(path),
   _system(system)
{
   try
   {
      openObject();
      writeHeader();
      _data->newData();
   }
   catch (...)
   {
      delete _file;
      delete _stream;
      delete _data;
      throw;
   }
}






/*!
 * Returns the path for this data object's file in absolute canonical terms. 
 *
 * @return Canonical path to this data object's file. 
 */
QString DataObject::path() const
{
   return _path;
}






/*!
 * Returns the data type for this data object. 
 *
 * @return Data object type. 
 */
quint16 DataObject::type() const
{
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
   return _file->size() - _headerOffset;
}






/*!
 * Returns the read only system metadata for this data object. The root metadata 
 * object is always an object type. 
 *
 * @return Root of system metadata for this data object. 
 */
EMetadata DataObject::systemMeta() const
{
   return _system;
}






/*!
 * Returns the user metadata for this data object. The root metadata object is 
 * always an object type. If reading the metadata failed then an exception is 
 * thrown. 
 *
 * @return Root of user metadata for this data object. 
 */
EMetadata DataObject::userMeta() const
{
   EMetadata ret;
   seek(_data->dataEnd());
   stream() >> ret;
   if ( stream().hasError() )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Read Error"));
      e.setDetails(tr("Failed reading in metadata for %1: %2")
                   .arg(_path)
                   .arg(stream().errorString()));
      throw e;
   }
   return ret;
}






/*!
 * Returns description of the last error that occurred from calling the read or 
 * write methods on this data object. If no error ever occurred on this data 
 * object then an empty string is returned. 
 *
 * @return Description of last error that occurred on read/write methods or 
 *         empty if no error has occurred. 
 */
QString DataObject::errorString() const
{
   return _file->errorString();
}






/*!
 * Seeks to the given index within this data object. This does not allow seeking 
 * to the hidden header of this data object so seeking to 0 is one byte after 
 * the header. If the given index is less than 0 then this does nothing and 
 * returns false. 
 *
 * @param index The index that the cursor position in this data object is set 
 *              to. 
 *
 * @return True on success or false on failure. 
 */
bool DataObject::seek(qint64 index) const
{
   if ( index < 0 )
   {
      return false;
   }
   return _file->seek(index + _headerOffset);
}






/*!
 * Allocates new space in this data object by the given number of bytes. The new 
 * space requested is in addition to the current cursor position of this data 
 * object. So if the cursor position is at the end of the data object then this 
 * will grow the data object by the given amount. However if the cursor position 
 * is at the beginning of the data object then this will only grow the data 
 * object by the different of the amount given and this data object's current 
 * size. 
 *
 * @param size The number of bytes to allocate after this data object's current 
 *             cursor position. 
 *
 * @return True on success or false on failure. 
 */
bool DataObject::allocate(int size)
{
   if ( size < 0 )
   {
      return false;
   }
   return _file->resize(_file->pos() + size);
}






/*!
 * Returns a read only reference to the data stream for this data object. 
 *
 * @return Read only reference to this objects data stream. 
 */
const EDataStream& DataObject::stream() const
{
   return *_stream;
}






/*!
 * Returns a reference to the data stream for this data object. 
 *
 * @return Reference to this objects data stream. 
 */
EDataStream& DataObject::stream()
{
   return *_stream;
}






/*!
 * Returns a reference to the abstract data object for this data object. 
 *
 * @return Reference to abstract data object. 
 */
EAbstractData& DataObject::data()
{
   return *_data;
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
   seek(_data->dataEnd());
   stream() << newRoot;
   if ( stream().hasError() )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Write Error"));
      e.setDetails(tr("Failed writing metadata to file %1: %2")
                   .arg(_path)
                   .arg(stream().errorString()));
      throw e;
   }
}






/*!
 *
 * @param data  
 *
 * @param size  
 */
qint64 DataObject::read(char* data, qint64 size) const
{
   if ( _file->pos() < _headerOffset )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logical Error"));
      e.setDetails(tr("Cannot read from header section of data object file."));
      throw e;
   }
   return _file->read(data,size);
}






/*!
 *
 * @param data  
 *
 * @param size  
 */
qint64 DataObject::write(const char* data, qint64 size)
{
   if ( _file->pos() < _headerOffset )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logical Error"));
      e.setDetails(tr("Cannot write to header section of data object file."));
      throw e;
   }
   return _file->write(data,size);
}






/*!
 */
void DataObject::openObject()
{
   _file = new QFile(_path,this);
   if ( !_file->open(QIODevice::ReadWrite) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("System Error"));
      e.setDetails(tr("Cannot open file %1; %2").arg(_path).arg(_file->errorString()));
      throw e;
   }
   _stream = new EDataStream(_file);//,this);///CHANGE TO HAVE PARENT!
   _path = QFileInfo(_path).canonicalPath();
}






/*!
 */
void DataObject::readHeader()
{
   qint64 value;
   QString name;
   QString extension;
   stream() >> value >> _type >> name >> extension >> _system;
   if ( stream().hasError() )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Read Error"));
      e.setDetails(tr("Failed reading in file %1: %2").arg(_path).arg(stream().errorString()));
      throw e;
   }
   if ( value != _specialValue )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Read Error"));
      e.setDetails(tr("Failed reading in file %1: it is not a data object file.").arg(_path));
      throw e;
   }
   makeData(name,extension);
   _headerOffset = _file->pos();
}






/*!
 */
void DataObject::writeHeader()
{
   EAbstractDataFactory& factory {EAbstractDataFactory::instance()};
   makeData(factory.name(_type),factory.fileExtension(_type));
   if ( !_file->resize(0) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("System Error"));
      e.setDetails(tr("Failed truncating file %1: %2").arg(_path).arg(_file->errorString()));
      throw e;
   }
   stream() << _specialValue
            << _type << factory.name(_type)
            << factory.fileExtension(_type)
            << _system;
   if ( stream().hasError() )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Write Error"));
      e.setDetails(tr("Failed writing to file %1: %2").arg(_path).arg(stream().errorString()));
      throw e;
   }
   _headerOffset = _file->pos();
}






/*!
 *
 * @param name  
 *
 * @param extension  
 */
void DataObject::makeData(const QString& name, const QString& extension)
{
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
