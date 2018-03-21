#include "edatastream.h"
#include <QtEndian>
#include <QPixmap>
#include "ace_dataobject.h"
//



/*!
 * Reads in data from this data stream setting the given value to the data read
 * in.
 *
 * @tparam T Data type for the given value that is read in from this data
 *           stream.
 *
 * @param value Reference to the value that is read in from this data stream.
 *
 * @return Read only reference to this data stream.
 */
template<class T> const EDataStream& EDataStream::read(T& value) const
{
   try
   {
      _parent->read(reinterpret_cast<char*>(&value),sizeof(T));
      value = qToBigEndian(value);
      return *this;
   }
   catch (...)
   {
      value = 0;
      throw;
   }
}






/*!
 * Writes out the given value to this data stream's data object.
 *
 * @tparam T The type for the given value that is written.
 *
 * @param value The value that is written to this data stream's data object.
 *
 * @return Reference to this data stream.
 */
template<class T> EDataStream& EDataStream::write(T value)
{
   value = qToBigEndian(value);
   _parent->write(reinterpret_cast<char*>(&value),sizeof(T));
   return *this;
}






/*!
 * This constructs a new data stream attached to the given data object where 
 * streamed data will be written to and read from. The given data object becomes 
 * this data stream's parent. 
 *
 * @param parent Pointer to the data object this data stream is attached to. 
 */
EDataStream::EDataStream(Ace::DataObject* parent)
{}






/*!
 * Reads in data from this data stream setting the given value to the data read 
 * in. If reading fails the given value is set to its default value. 
 *
 * @param value Reference to value that is set to the value read in from this 
 *              data stream. 
 *
 * @return Read only reference to this data stream. 
 */
const EDataStream& EDataStream::operator>>(qint8& value) const
{
   return read(value);
}






/*!
 * Reads in data from this data stream setting the given value to the data read 
 * in. If reading fails the given value is set to its default value. 
 *
 * @param value Reference to value that is set to the value read in from this 
 *              data stream. 
 *
 * @return Read only reference to this data stream. 
 */
const EDataStream& EDataStream::operator>>(qint16& value) const
{}






/*!
 * Reads in data from this data stream setting the given value to the data read 
 * in. If reading fails the given value is set to its default value. 
 *
 * @param value Reference to value that is set to the value read in from this 
 *              data stream. 
 *
 * @return Read only reference to this data stream. 
 */
const EDataStream& EDataStream::operator>>(qint32& value) const
{}






/*!
 * Reads in data from this data stream setting the given value to the data read 
 * in. If reading fails the given value is set to its default value. 
 *
 * @param value Reference to value that is set to the value read in from this 
 *              data stream. 
 *
 * @return Read only reference to this data stream. 
 */
const EDataStream& EDataStream::operator>>(qint64& value) const
{}






/*!
 * Reads in data from this data stream setting the given value to the data read 
 * in. If reading fails the given value is set to its default value. 
 *
 * @param value Reference to value that is set to the value read in from this 
 *              data stream. 
 *
 * @return Read only reference to this data stream. 
 */
const EDataStream& EDataStream::operator>>(quint8& value) const
{}






/*!
 * Reads in data from this data stream setting the given value to the data read 
 * in. If reading fails the given value is set to its default value. 
 *
 * @param value Reference to value that is set to the value read in from this 
 *              data stream. 
 *
 * @return Read only reference to this data stream. 
 */
const EDataStream& EDataStream::operator>>(quint16& value) const
{}






/*!
 * Reads in data from this data stream setting the given value to the data read 
 * in. If reading fails the given value is set to its default value. 
 *
 * @param value Reference to value that is set to the value read in from this 
 *              data stream. 
 *
 * @return Read only reference to this data stream. 
 */
const EDataStream& EDataStream::operator>>(quint32& value) const
{}






/*!
 * Reads in data from this data stream setting the given value to the data read 
 * in. If reading fails the given value is set to its default value. 
 *
 * @param value Reference to value that is set to the value read in from this 
 *              data stream. 
 *
 * @return Read only reference to this data stream. 
 */
const EDataStream& EDataStream::operator>>(quint64& value) const
{}






/*!
 * Reads in data from this data stream setting the given value to the data read 
 * in. If reading fails the given value is set to its default value. 
 *
 * @param value Reference to value that is set to the value read in from this 
 *              data stream. 
 *
 * @return Read only reference to this data stream. 
 */
const EDataStream& EDataStream::operator>>(float& value) const
{
   return read(value);
}






/*!
 * Reads in data from this data stream setting the given value to the data read 
 * in. If reading fails the given value is set to its default value. 
 *
 * @param value Reference to value that is set to the value read in from this 
 *              data stream. 
 *
 * @return Read only reference to this data stream. 
 */
const EDataStream& EDataStream::operator>>(double& value) const
{}






/*!
 * Reads in data from this data stream setting the given value to the data read 
 * in. If reading fails the given value is set to its default value. 
 *
 * @param value Reference to value that is set to the value read in from this 
 *              data stream. 
 *
 * @return Read only reference to this data stream. 
 */
const EDataStream& EDataStream::operator>>(QString& value) const
{
   // clear input string
   value.clear();

   // make sure stream is in ok state
   if ( *this )
   {
      // read type identifier
      quint8 type;
      *this >> type;
      if ( *this )
      {
         // make sure it is correct type
         if ( type != String )
         {
            E_MAKE_EXCEPTION(e);
            e.setTitle(QObject::tr("Data Stream Read"));
            e.setDetails(QObject::tr("Could not read string because data is corrupt"));
            setException(e);
         }
         else
         {
            // read in string in UTF8 format
            QByteArray data;
            *this >> data;
            value = QString::fromUtf8(data);
         }
      }
   }

   // return reference to stream
   return *this;
}






/*!
 * Reads in data from this data stream setting the given value to the data read 
 * in. If reading fails the given value is set to its default value. The given 
 * byte array should not have any endian sensitive data stored within it because 
 * this does not handle it at all. 
 *
 * @param value Reference to value that is set to the value read in from this 
 *              data stream. 
 *
 * @return Read only reference to this data stream. 
 */
const EDataStream& EDataStream::operator>>(QByteArray& value) const
{
   // clear input
   value.clear();

   // make sure stream is in ok state
   if ( *this )
   {
      // read type identifier
      quint8 type;
      *this >> type;
      if ( *this )
      {
         // make sure it is correct type
         if ( type != ByteArray )
         {
            E_MAKE_EXCEPTION(e);
            e.setTitle(QObject::tr("Data Stream Read"));
            e.setDetails(QObject::tr("Could not read byte array because data is corrupt."));
            setException(e);
         }
         else
         {
            // read size of byte array
            quint32 size;
            *this >> size;
            if ( *this )
            {
               // read byte array and make sure it worked
               value.resize(size);
               if ( !rawRead(value.data(),size) )
               {
                  E_MAKE_EXCEPTION(e);
                  e.setTitle(QObject::tr("Data Stream Read"));
                  e.setDetails(QObject::tr("Could not read byte array because data is corrupt."));
                  setException(e);
               }
            }
         }
      }
   }

   // return reference to stream
   return *this;
}






/*!
 * Writes out the given value to this data stream. 
 *
 * @param value The value that is written to this data stream. 
 *
 * @return Reference to this data stream. 
 */
EDataStream& EDataStream::operator<<(qint8 value)
{
   return write(value);
}






/*!
 * Writes out the given value to this data stream. 
 *
 * @param value The value that is written to this data stream. 
 *
 * @return Reference to this data stream. 
 */
EDataStream& EDataStream::operator<<(qint16 value)
{}






/*!
 * Writes out the given value to this data stream. 
 *
 * @param value The value that is written to this data stream. 
 *
 * @return Reference to this data stream. 
 */
EDataStream& EDataStream::operator<<(qint32 value)
{}






/*!
 * Writes out the given value to this data stream. 
 *
 * @param value The value that is written to this data stream. 
 *
 * @return Reference to this data stream. 
 */
EDataStream& EDataStream::operator<<(qint64 value)
{}






/*!
 * Writes out the given value to this data stream. 
 *
 * @param value The value that is written to this data stream. 
 *
 * @return Reference to this data stream. 
 */
EDataStream& EDataStream::operator<<(quint8 value)
{}






/*!
 * Writes out the given value to this data stream. 
 *
 * @param value The value that is written to this data stream. 
 *
 * @return Reference to this data stream. 
 */
EDataStream& EDataStream::operator<<(quint16 value)
{}






/*!
 * Writes out the given value to this data stream. 
 *
 * @param value The value that is written to this data stream. 
 *
 * @return Reference to this data stream. 
 */
EDataStream& EDataStream::operator<<(quint32 value)
{}






/*!
 * Writes out the given value to this data stream. 
 *
 * @param value The value that is written to this data stream. 
 *
 * @return Reference to this data stream. 
 */
EDataStream& EDataStream::operator<<(quint64 value)
{}






/*!
 * Writes out the given value to this data stream. 
 *
 * @param value The value that is written to this data stream. 
 *
 * @return Reference to this data stream. 
 */
EDataStream& EDataStream::operator<<(float value)
{}






/*!
 * Writes out the given value to this data stream. 
 *
 * @param value The value that is written to this data stream. 
 *
 * @return Reference to this data stream. 
 */
EDataStream& EDataStream::operator<<(double value)
{}






/*!
 * Writes out the given value to this data stream. 
 *
 * @param value The value that is written to this data stream. 
 *
 * @return Reference to this data stream. 
 */
EDataStream& EDataStream::operator<<(const QString& value)
{
   // make sure stream is in ok state
   if ( *this )
   {
      // write out string identifier type
      quint8 type = String;
      *this << type;
      if ( *this )
      {
         // write out string in UTF8 format
         *this << value.toUtf8();
      }
   }

   // return reference to stream
   return *this;
}






/*!
 * Writes out the given value to this data stream. The given byte array should 
 * not have any endian sensitive data stored within it because this does not 
 * handle it at all. 
 *
 * @param value The value that is written to this data stream. 
 *
 * @return Reference to this data stream. 
 */
EDataStream& EDataStream::operator<<(const QByteArray& value)
{
   // make sure stream is in ok state
   if ( *this )
   {
      // write out byte array identifier type
      quint8 type = ByteArray;
      *this << type;
      if ( *this )
      {
         // write out size of byte array
         quint32 size = value.size();
         *this << size;
         if ( *this )
         {
            // write out byte array
            rawWrite(value.data(),value.size());
         }
      }
   }

   // return reference to stream
   return *this;
}






/*! !!! UNKNOWN FUNCTION !!! */
bool EDataStream::rawWrite(const void *data, int size)
{
   // write value to file checking if it worked
   if ( _file->write(reinterpret_cast<const char*>(data),size) != size )
   {
      // set stream to error state and return failure
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Data Stream Write"));
      e.setDetails(QObject::tr("Failed writing to file: %1").arg(_file->errorString()));
      setException(e);
      return false;
   }

   // return success
   return true;
}






/*! !!! UNKNOWN FUNCTION !!! */
bool EDataStream::rawRead(void *data, int size) const
{
   // read value from file checking if it worked
   if ( _file->read(reinterpret_cast<char*>(data),size) != size )
   {
      // set stream to error state and return failure
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Data Stream Read"));
      e.setDetails(QObject::tr("Failed reading from file: %1").arg(_file->errorString()));
      setException(e);
      return false;
   }

   // return success
   return true;
}
