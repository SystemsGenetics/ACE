#include "emetadata.h"
#include <QString>
#include <QByteArray>
#include "exception.h"
#include "emetaarray.h"
#include "emetaobject.h"
//






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
   case Array:
      _data = new EMetaArray;
      break;
   case Object:
      _data = new EMetaObject;
      break;
   default:
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
 *
 * 3. Return reference to this object. 
 */
EMetadata& EMetadata::operator=(const EMetadata& object)
{
   clear();
   _type = object._type;
   copy(object._data);
   return *this;
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
 *
 * 4. Return reference to this object. 
 */
EMetadata& EMetadata::operator=(EMetadata&& object)
{
   clear();
   _type = object._type;
   _data = object._data;
   object._type = Null;
   object._data = nullptr;
   return *this;
}






/*!
 */
EMetadata::~EMetadata()
{
   clear();
}






/*!
 */
bool EMetadata::isNull() const
{
   return _type == Null;
}






/*!
 */
bool EMetadata::isBool() const
{
   return _type == Bool;
}






/*!
 */
bool EMetadata::isDouble() const
{
   return _type == Double;
}






/*!
 */
bool EMetadata::isString() const
{
   return _type == String;
}






/*!
 */
bool EMetadata::isBytes() const
{
   return _type == Bytes;
}






/*!
 */
bool EMetadata::isArray() const
{
   return _type == Array;
}






/*!
 */
bool EMetadata::isObject() const
{
   return _type == Object;
}






/*!
 */
bool& EMetadata::toBool()
{
   checkType(Bool);
   return *static_cast<bool*>(_data);
}






/*!
 */
const bool& EMetadata::toBool() const
{
   checkType(Bool);
   return *static_cast<bool*>(_data);
}






/*!
 */
double& EMetadata::toDouble()
{
   checkType(Double);
   return *static_cast<double*>(_data);
}






/*!
 */
const double& EMetadata::toDouble() const
{
   checkType(Double);
   return *static_cast<double*>(_data);
}






/*!
 */
QString& EMetadata::toString()
{
   checkType(String);
   return *static_cast<QString*>(_data);
}






/*!
 */
const QString& EMetadata::toString() const
{
   checkType(String);
   return *static_cast<QString*>(_data);
}






/*!
 */
QByteArray& EMetadata::toBytes()
{
   checkType(Bytes);
   return *static_cast<QByteArray*>(_data);
}






/*!
 */
const QByteArray& EMetadata::toBytes() const
{
   checkType(Bytes);
   return *static_cast<QByteArray*>(_data);
}






/*!
 */
EMetaArray& EMetadata::toArray()
{
   checkType(Array);
   return *static_cast<EMetaArray*>(_data);
}






/*!
 */
const EMetaArray& EMetadata::toArray() const
{
   checkType(Array);
   return *static_cast<EMetaArray*>(_data);
}






/*!
 */
EMetaObject& EMetadata::toObject()
{
   checkType(Object);
   return *static_cast<EMetaObject*>(_data);
}






/*!
 */
const EMetaObject& EMetadata::toObject() const
{
   checkType(Object);
   return *static_cast<EMetaObject*>(_data);
}






/*!
 */
EMetadata::Type EMetadata::type() const
{
   return _type;
}






/*!
 * Returns the name of the given type as a string. 
 *
 * @param type Type to be given name of. 
 *
 * @return Name of the given type. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Return constant string of correct type name from given type. 
 */
QString EMetadata::typeName(Type type)
{
   switch (type)
   {
   case Null: return "null";
   case Bool: return "bool";
   case Double: return "double";
   case String: return "string";
   case Bytes: return "bytes";
   case Array: return "array";
   case Object: return "object";
   default: return QString();
   }
}






/*!
 * Makes sure given type matches the type this object contains. If it does not 
 * match an exception is thrown saying so. 
 *
 * @param type Given type to verify it is this object's type. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given type does not match with this object's type then throw an 
 *    exception, else return from operation. 
 */
void EMetadata::checkType(Type type) const
{
   if ( _type != type )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Logical Error"));
      e.setDetails(
               QObject::tr("Cannot convert metadata to %1 type when it is %2 type.")
               .arg(typeName(type))
               .arg(typeName(_type)));
      throw e;
   }
}






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
   case Array:
      delete static_cast<EMetaArray*>(_data);
      break;
   case Object:
      delete static_cast<EMetaObject*>(_data);
      break;
   default:
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
      break;
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
   case Array:
      *static_cast<EMetaArray*>(_data) = *static_cast<const EMetaArray*>(data);
      break;
   case Object:
      *static_cast<EMetaObject*>(_data) = *static_cast<const EMetaObject*>(data);
      break;
   }
}
