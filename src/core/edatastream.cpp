#include "edatastream.h"
#include <QtEndian>
#include <QPixmap>
#include "ace_dataobject.h"
#include "exception.h"
//






/*!
 * This constructs a new data stream attached to the given data object where 
 * streamed data will be written to and read from. The given data object becomes 
 * this data stream's parent. 
 *
 * @param parent Pointer to the data object this data stream is attached to. 
 */
EDataStream::EDataStream(Ace::DataObject* parent):
   _parent(parent)
{}






/*!
 * Reads in data from this data stream setting the given value to the data read 
 * in. 
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
 * in. 
 *
 * @param value Reference to value that is set to the value read in from this 
 *              data stream. 
 *
 * @return Read only reference to this data stream. 
 */
const EDataStream& EDataStream::operator>>(qint16& value) const
{
   return read(value);
}






/*!
 * Reads in data from this data stream setting the given value to the data read 
 * in. 
 *
 * @param value Reference to value that is set to the value read in from this 
 *              data stream. 
 *
 * @return Read only reference to this data stream. 
 */
const EDataStream& EDataStream::operator>>(qint32& value) const
{
   return read(value);
}






/*!
 * Reads in data from this data stream setting the given value to the data read 
 * in. 
 *
 * @param value Reference to value that is set to the value read in from this 
 *              data stream. 
 *
 * @return Read only reference to this data stream. 
 */
const EDataStream& EDataStream::operator>>(qint64& value) const
{
   return read(value);
}






/*!
 * Reads in data from this data stream setting the given value to the data read 
 * in. 
 *
 * @param value Reference to value that is set to the value read in from this 
 *              data stream. 
 *
 * @return Read only reference to this data stream. 
 */
const EDataStream& EDataStream::operator>>(quint8& value) const
{
   return read(value);
}






/*!
 * Reads in data from this data stream setting the given value to the data read 
 * in. 
 *
 * @param value Reference to value that is set to the value read in from this 
 *              data stream. 
 *
 * @return Read only reference to this data stream. 
 */
const EDataStream& EDataStream::operator>>(quint16& value) const
{
   return read(value);
}






/*!
 * Reads in data from this data stream setting the given value to the data read 
 * in. 
 *
 * @param value Reference to value that is set to the value read in from this 
 *              data stream. 
 *
 * @return Read only reference to this data stream. 
 */
const EDataStream& EDataStream::operator>>(quint32& value) const
{
   return read(value);
}






/*!
 * Reads in data from this data stream setting the given value to the data read 
 * in. 
 *
 * @param value Reference to value that is set to the value read in from this 
 *              data stream. 
 *
 * @return Read only reference to this data stream. 
 */
const EDataStream& EDataStream::operator>>(quint64& value) const
{
   return read(value);
}






/*!
 * Reads in data from this data stream setting the given value to the data read 
 * in. 
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
 * in. 
 *
 * @param value Reference to value that is set to the value read in from this 
 *              data stream. 
 *
 * @return Read only reference to this data stream. 
 */
const EDataStream& EDataStream::operator>>(double& value) const
{
   return read(value);
}






/*!
 * Reads in data from this data stream setting the given value to the data read 
 * in. 
 *
 * @param value Reference to value that is set to the value read in from this 
 *              data stream. 
 *
 * @return Read only reference to this data stream. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Read in the type marker from the data stream. If the type is not a string 
 *    then throw an exception, else go to the next step. 
 *
 * 2. Read in the string as a byte array and then set the given string's value 
 *    to the byte array assuming it is encoded with UTF-8. 
 *
 * 3. Return reference to this data stream. 
 */
const EDataStream& EDataStream::operator>>(QString& value) const
{
   quint8 type;
   *this >> type;
   if ( type != static_cast<quint8>(Tag::String) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Read Error"));
      e.setDetails(tr("Could not read string from data stream because data is corrupt"));
      throw e;
   }
   QByteArray data;
   *this >> data;
   value = QString::fromUtf8(data);
   return *this;
}






/*!
 * Reads in data from this data stream setting the given value to the data read 
 * in. The given byte array should not have any endian sensitive data stored 
 * within it because this does not handle it at all. 
 *
 * @param value Reference to value that is set to the value read in from this 
 *              data stream. 
 *
 * @return Read only reference to this data stream. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Read in type marker from the data stream. If the type is not a byte array 
 *    then throw an exception, else go to the next step. 
 *
 * 2. Read in the size of the byte array and then read in the byte array itself, 
 *    setting the given byte array to the read in value. 
 *
 * 3. Return reference to this data stream. 
 */
const EDataStream& EDataStream::operator>>(QByteArray& value) const
{
   quint8 type;
   *this >> type;
   if ( type != static_cast<quint8>(Tag::ByteArray) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Read Error"));
      e.setDetails(tr("Could not read byte array from data stream because data is corrupt."));
      throw e;
   }
   quint32 size;
   *this >> size;
   value.clear();
   value.resize(size);
   _parent->read(value.data(),size);
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
EDataStream& EDataStream::operator<<(qint32 value)
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
EDataStream& EDataStream::operator<<(qint64 value)
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
EDataStream& EDataStream::operator<<(quint8 value)
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
EDataStream& EDataStream::operator<<(quint16 value)
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
EDataStream& EDataStream::operator<<(quint32 value)
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
EDataStream& EDataStream::operator<<(quint64 value)
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
EDataStream& EDataStream::operator<<(float value)
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
EDataStream& EDataStream::operator<<(double value)
{
   return write(value);
}






/*!
 * Writes out the given value to this data stream. 
 *
 * @param value The value that is written to this data stream. 
 *
 * @return Reference to this data stream. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Write out the string type marker and the string itself as a byte array 
 *    encoded with UTF-8. 
 *
 * 2. Return a reference to this data stream. 
 */
EDataStream& EDataStream::operator<<(const QString& value)
{
   *this << static_cast<quint8>(Tag::String);
   *this << value.toUtf8();
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
 *
 *
 * Steps of Operation: 
 *
 * 1. Write out the byte array type marker, the size of the byte array, and the 
 *    byte array itself. 
 *
 * 2. Return a reference to this data stream. 
 */
EDataStream& EDataStream::operator<<(const QByteArray& value)
{
   *this << static_cast<quint8>(Tag::ByteArray);
   *this << static_cast<quint32>(value.size());
   _parent->write(value.data(),value.size());
   return *this;
}






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
 *
 *
 * Steps of Operation: 
 *
 * 1. Read in the given value from this data stream, flipping the bytes of the 
 *    read in value if required. 
 *
 * 2. Return a reference to this data stream. 
 */
template<class T> const EDataStream& EDataStream::read(T& value) const
{
   _parent->read(reinterpret_cast<char*>(&value),sizeof(T));
   value = qFromBigEndian(value);
   return *this;
}






/*!
 * Writes out the given value to this data stream's data object. 
 *
 * @tparam T The type for the given value that is written. 
 *
 * @param value The value that is written to this data stream's data object. 
 *
 * @return Reference to this data stream. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Write out the given value to the data stream, flipping the bytes before 
 *    writing if required. 
 *
 * 2. Return a reference to this data stream. 
 */
template<class T> EDataStream& EDataStream::write(T value)
{
   value = qToBigEndian(value);
   _parent->write(reinterpret_cast<char*>(&value),sizeof(T));
   return *this;
}
