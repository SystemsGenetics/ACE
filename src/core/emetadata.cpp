#include "emetadata.h"
#include <QString>
#include <QByteArray>






/*!
 * This creates a new metadata object of the type specified. 
 *
 * @param type The type this new object will become. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Based off the type of object create new data of the given type and store 
 *    its pointer. If the type is null simply leave the data pointer as null. 
 */
EMetadata::EMetadata(Type type):
   _type(type)
{
   switch (_type)
   {
   case Bool:
      _data = new bool;
      break;
   case Double:
      _data = new double;
      break;
   case String:
      _data = new QString;
      break;
   case Bytes:
      _data = new QByteArray;
      break;
   }
}






/*!
 * This creates a new metadata object that is a direct copy of the one supplied. 
 *
 * @param object The metadata object that is copied. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Copy the type and data of the supplied metadata object. 
 */
EMetadata::EMetadata(const EMetadata& object):
   _type(object._type)
{
   copy(object._data);
}






/*!
 * This creates a new metadata object that takes possession of the data of the 
 * one supplied as an argument. The argument's type is changed to Null. 
 *
 * @param object The metadata object whose data is taken. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Copy the type and data pointer of the supplied metadata object. 
 *
 * 2. Set the supplied metadata object to Null. 
 */
EMetadata::EMetadata(EMetadata&& object):
   _type(object._type),
   _data(object._data)
{
   object._type = Null;
   object._data = nullptr;
}






/*!
 * Set this object's data as a copy of the supplied metadata object. 
 *
 * @param object The metadata object that is copied. 
 *
 * @return Reference to this object. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Delete any data this object contains. 
 *
 * 2. Copy the type and data of the supplied metadata object. 
 */
EMetadata& EMetadata::operator=(const EMetadata& object)
{
   clear();
   _type = object._type;
   copy(object._data);
}






/*!
 * Take the data of another metadata object and set this object's data with it. 
 *
 * @param object The metadata object whose data is taken. 
 *
 * @return Reference to this object. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Delete any data this object contains. 
 *
 * 2. Copy the type and data pointer of the supplied metadata object. 
 *
 * 3. Set the supplied metadata object to Null. 
 */
EMetadata& EMetadata::operator=(EMetadata&& object)
{
   clear();
   _type = object._type;
   _data = object._data;
   object._type = Null;
   object._data = nullptr;
}






/*!
 */
EMetadata::~EMetadata()
{}






/*!
 */
bool EMetadata::isNull() const
{}






/*!
 */
bool EMetadata::isBool() const
{}






/*!
 */
bool EMetadata::isDouble() const
{}






/*!
 */
bool EMetadata::isString() const
{}






/*!
 */
bool EMetadata::isBytes() const
{}






/*!
 */
bool EMetadata::isArray() const
{}






/*!
 */
bool EMetadata::isObject() const
{}






/*!
 */
bool& EMetadata::toBool()
{}






/*!
 */
const bool& EMetadata::toBool() const
{}






/*!
 */
double& EMetadata::toDouble()
{}






/*!
 */
const double& EMetadata::toDouble() const
{}






/*!
 */
QString& EMetadata::toString()
{}






/*!
 */
const QString& EMetadata::toString() const
{}






/*!
 */
QByteArray& EMetadata::toBytes()
{}






/*!
 */
const QByteArray& EMetadata::toBytes() const
{}






/*!
 */
EMetaArray& EMetadata::toArray()
{}






/*!
 */
const EMetaArray& EMetadata::toArray() const
{}






/*!
 */
EMetaObject& EMetadata::toObject()
{}






/*!
 */
const EMetaObject& EMetadata::toObject() const
{}






/*!
 */
EMetadata::Type EMetadata::type() const
{}






/*!
 */
QString EMetadata::typeName() const
{}






/*!
 * Deletes this object's data. Does not alter the type or data pointer of the 
 * object. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Delete the underlying data the data pointer points to, if any. 
 */
void EMetadata::clear()
{
   switch (_type)
   {
   case Bool:
      delete static_cast<bool*>(_data);
      break;
   case Double:
      delete static_cast<double*>(_data);
      break;
   case String:
      delete static_cast<QString*>(_data);
      break;
   case Bytes:
      delete static_cast<QByteArray*>(_data);
      break;
   }
}






/*!
 * Copy data from the supplied data pointer to the object's data pointer based 
 * off this object's type. If the type is Null then this object's data pointer 
 * is set to Null. 
 *
 * @param data The data pointer to use for possible copying. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Copy the data pointed to by this object's data pointer from a data pointed 
 *    to by the supplied data pointer based of this object's type. If the type 
 *    is Null then the data pointer is set to null and the supplied data pointer 
 *    is ignored. 
 */
void EMetadata::copy(const void* data)
{
   switch (_type)
   {
   case Null:
      _data = nullptr;
   case Bool:
      *static_cast<bool*>(_data) = *static_cast<const bool*>(data);
      break;
   case Double:
      *static_cast<double*>(_data) = *static_cast<const double*>(data);
      break;
   case String:
      *static_cast<QString*>(_data) = *static_cast<const QString*>(data);
      break;
   case Bytes:
      *static_cast<QByteArray*>(_data) = *static_cast<const QByteArray*>(data);
      break;
   }
}

