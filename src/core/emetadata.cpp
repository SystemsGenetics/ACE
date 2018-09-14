#include "emetadata.h"
#include <QString>
#include <QByteArray>
#include <QJsonDocument>
#include "eexception.h"
#include "emetaarray.h"
#include "emetaobject.h"
#include "edebug.h"



//






/*!
 * Set this object's data as a copy of the supplied metadata object. 
 *
 * @param object The metadata object that is copied. 
 *
 * @return Reference to this object. 
 */
EMetadata& EMetadata::operator=(const EMetadata& object)
{
   EDEBUG_FUNC(this,object)

   // Delete any data this object contains. 
   clear();

   // Copy the type and data of the supplied metadata object. 
   _type = object._type;
   create();
   copy(object._data);

   // Return reference to this object. 
   return *this;
}






/*!
 * Take the data of another metadata object and set this object's data with it. 
 *
 * @param object The metadata object whose data is taken. 
 *
 * @return Reference to this object. 
 */
EMetadata& EMetadata::operator=(EMetadata&& object)
{
   EDEBUG_FUNC(this,object)

   // Delete any data this object contains. 
   clear();

   // Copy the type and data pointer of the supplied metadata object. 
   _type = object._type;
   _data = object._data;

   // Set the supplied metadata object to Null. 
   object._type = Null;
   object._data = nullptr;

   // Return reference to this object. 
   return *this;
}






/*!
 * Constructs a new metadata object of the type specified. 
 *
 * @param type The type this new object will become. 
 */
EMetadata::EMetadata(Type type):
   _type(type)
{
   EDEBUG_FUNC(this,type)

   create();
}






/*!
 * Constructs a new metadata object of the double type, setting its initial value 
 * to the one given. 
 *
 * @param value Initial value of this new metadata object. 
 */
EMetadata::EMetadata(double value):
   EMetadata(Double)
{
   EDEBUG_FUNC(this,value)

   toDouble() = value;
}






/*!
 * Constructs a new metadata object of the string type, setting its initial value 
 * to the one given. 
 *
 * @param value Initial value of this new metadata object. 
 */
EMetadata::EMetadata(const QString& value):
   EMetadata(String)
{
   EDEBUG_FUNC(this,value)

   toString() = value;
}






/*!
 * Constructs a new metadata object of the byte array type, setting its initial 
 * value to the one given. 
 *
 * @param value Initial value of this new metadata object. 
 */
EMetadata::EMetadata(const QByteArray& value):
   EMetadata(Bytes)
{
   EDEBUG_FUNC(this,&value)

   toBytes() = value;
}






/*!
 * Constructs a new metadata object of the array type, setting its initial value to 
 * the one given. 
 *
 * @param value Initial value of this new metadata object. 
 */
EMetadata::EMetadata(const EMetaArray& value):
   EMetadata(Array)
{
   EDEBUG_FUNC(this,value)

   toArray() = value;
}






/*!
 * Constructs a new metadata object of the object type, setting its initial value 
 * to the one given. 
 *
 * @param value Initial value of this new metadata object. 
 */
EMetadata::EMetadata(const EMetaObject& value):
   EMetadata(Object)
{
   EDEBUG_FUNC(this,value)

   toObject() = value;
}






/*!
 * Constructs a new metadata object from the given JSON value. If the JSON is an 
 * array or object this will recursively create all children metadata mirroring the 
 * structure of the given JSON. 
 *
 * @param value JSON value that is used to construct this new metadata object. 
 */
EMetadata::EMetadata(const QJsonValue& value)
{
   EDEBUG_FUNC(this,&value)

   // If the given json value is a boolean, double, or string then initialize this 
   // metadata object to that type, set its value to the json value, and exit, else 
   // go to the next step. 
   if ( value.isBool() )
   {
      *this = EMetadata(Bool);
      toBool() = value.toBool();
   }
   else if ( value.isDouble() )
   {
      *this = EMetadata(Double);
      toDouble() = value.toDouble();
   }
   else if ( value.isString() )
   {
      *this = EMetadata(String);
      toString() = value.toString();
   }

   // If the given json value is an array then initialize this metadata object to 
   // that type, copy all children json values to this metadata object as children 
   // metadata, and exit, else go to the next step. 
   else if ( value.isArray() )
   {
      *this = EMetadata(Array);
      const QJsonArray values {value.toArray()};
      for (auto value: values)
      {
         toArray().append(EMetadata(value));
      }
   }

   // If the given json value is an object then initialize this metadata object to 
   // that type, copy all children json values with their associated keys to this 
   // metadata object as children metadata with the same keys, and exit, else go to 
   // the next step. 
   else if ( value.isObject() )
   {
      *this = EMetadata(Object);
      const QJsonObject values {value.toObject()};
      for (auto i = values.begin(); i != values.end() ;++i)
      {
         toObject().insert(i.key(),EMetadata(*i));
      }
   }

   // If this is reached then initialize this metadata object to the null type. 
   else
   {
      *this = EMetadata(Null);
   }
}






/*!
 * This creates a new metadata object that is a direct copy of the one supplied. 
 *
 * @param object The metadata object that is copied. 
 */
EMetadata::EMetadata(const EMetadata& object):
   _type(object._type)
{
   // Copy the type and data of the supplied metadata object. 
   create();
   copy(object._data);
}






/*!
 * This creates a new metadata object that takes possession of the data of the one 
 * supplied as an argument. The argument's type is changed to Null. 
 *
 * @param object The metadata object whose data is taken. 
 */
EMetadata::EMetadata(EMetadata&& object):
   _type(object._type),
   _data(object._data)
{
   EDEBUG_FUNC(this,object)

   // Set the supplied metadata object to Null. 
   object._type = Null;
   object._data = nullptr;
}






/*!
 * Deletes any data this object may contain. 
 */
EMetadata::~EMetadata()
{
   EDEBUG_FUNC(this)

   clear();
}






/*!
 * Returns the JSON equivalent to this metadata excluding any byte array types 
 * because JSON does not support that. If this metadata is an array or object then 
 * all children are recursively copied into JSON format and returned with the root 
 * JSON value. 
 *
 * @return JSON value of this metadata object. 
 */
QJsonValue EMetadata::toJson() const
{
   EDEBUG_FUNC(this)

   switch (_type)
   {
   // If this metadata is a boolean, double, or string then return the JSON value of 
   // this metadata object, else go to the next step. 
   case Bool:
      return QJsonValue(toBool());
   case Double:
      return QJsonValue(toDouble());
   case String:
      return QJsonValue(toString());

   // If this metadata is an array then return a JSON array of this metadata object 
   // with all metadata children recursively added as JSON values themselves, else go 
   // to the next step. 
   case Array:
      {
         QJsonArray ret;
         for (auto value: toArray())
         {
            ret.append(value.toJson());
         }
         return ret;
      }

   // If this metadata is an object then return a JSON object of this metadata object 
   // with all metadata children recursively added as JSON values and associated 
   // their keys, else go to the next step. 
   case Object:
      {
         QJsonObject ret;
         for (auto i = toObject().begin(); i != toObject().end() ;++i)
         {
            ret.insert(i.key(),i->toJson());
         }
         return ret;
      }

   // If this is reached then metadata is a byte array or null so return an null JSON 
   // value. 
   default:
      return QJsonValue();
   }
}






/*!
 * Returns the type for this metadata. 
 *
 * @return This type for this metadata. 
 */
EMetadata::Type EMetadata::type() const
{
   return _type;
}






/*!
 * Tests if this metadata is a null type. 
 *
 * @return True if this metadata is a null type or false otherwise. 
 */
bool EMetadata::isNull() const
{
   EDEBUG_FUNC(this)

   return _type == Null;
}






/*!
 * Tests if this metadata is a boolean type. 
 *
 * @return True if this metadata is a boolean type or false otherwise. 
 */
bool EMetadata::isBool() const
{
   EDEBUG_FUNC(this)

   return _type == Bool;
}






/*!
 * Tests if this metadata is a double (real number) type. 
 *
 * @return True if this metadata is a double type or false otherwise. 
 */
bool EMetadata::isDouble() const
{
   EDEBUG_FUNC(this)

   return _type == Double;
}






/*!
 * Tests if this metadata is a string type. 
 *
 * @return True if this metadata is a string type or false otherwise. 
 */
bool EMetadata::isString() const
{
   EDEBUG_FUNC(this)

   return _type == String;
}






/*!
 * Tests if this metadata is a byte array type. 
 *
 * @return True if this metadata is a byte type or false otherwise. 
 */
bool EMetadata::isBytes() const
{
   EDEBUG_FUNC(this)

   return _type == Bytes;
}






/*!
 * Tests if this metadata is an array type. 
 *
 * @return True if this metadata is an array type or false otherwise. 
 */
bool EMetadata::isArray() const
{
   EDEBUG_FUNC(this)

   return _type == Array;
}






/*!
 * Tests if this metadata is an object type. 
 *
 * @return True if this metadata is an object type. 
 */
bool EMetadata::isObject() const
{
   EDEBUG_FUNC(this)

   return _type == Object;
}






/*!
 * Returns a read only reference to this object's data cast as a boolean. If This 
 * metadata is not a boolean type then an exception is thrown. 
 *
 * @return Read only reference to this object's boolean data. 
 */
const bool& EMetadata::toBool() const
{
   EDEBUG_FUNC(this)

   checkType(Bool);
   return *static_cast<bool*>(_data);
}






/*!
 * Returns a read only reference to this object's data cast as a double. If This 
 * metadata is not a double type then an exception is thrown. 
 *
 * @return Read only reference to this object's double data. 
 */
const double& EMetadata::toDouble() const
{
   EDEBUG_FUNC(this)

   checkType(Double);
   return *static_cast<double*>(_data);
}






/*!
 * Returns a read only reference to this object's data cast as a string. If This 
 * metadata is not a string type then an exception is thrown. 
 *
 * @return Read only reference to this object's string data. 
 */
const QString& EMetadata::toString() const
{
   EDEBUG_FUNC(this)

   checkType(String);
   return *static_cast<QString*>(_data);
}






/*!
 * Returns a read only reference to this object's data cast as a byte array. If 
 * This metadata is not a byte type then an exception is thrown. 
 *
 * @return Read only reference to this object's byte array data. 
 */
const QByteArray& EMetadata::toBytes() const
{
   EDEBUG_FUNC(this)

   checkType(Bytes);
   return *static_cast<QByteArray*>(_data);
}






/*!
 * Returns a read only reference to this object's data cast as a meta array. If 
 * This metadata is not an array type then an exception is thrown. 
 *
 * @return Read only reference to this object's meta array. 
 */
const EMetaArray& EMetadata::toArray() const
{
   EDEBUG_FUNC(this)

   checkType(Array);
   return *static_cast<EMetaArray*>(_data);
}






/*!
 * Returns a read only reference to this object's data cast as a meta object. If 
 * This metadata is not an object type then an exception is thrown. 
 *
 * @return Read only reference to this object's meta object. 
 */
const EMetaObject& EMetadata::toObject() const
{
   EDEBUG_FUNC(this)

   checkType(Object);
   return *static_cast<EMetaObject*>(_data);
}






/*!
 * Returns a reference to this object's data cast as a boolean. If This metadata is 
 * not a boolean type then an exception is thrown. 
 *
 * @return Reference to this object's boolean data. 
 */
bool& EMetadata::toBool()
{
   EDEBUG_FUNC(this)

   checkType(Bool);
   return *static_cast<bool*>(_data);
}






/*!
 * Returns a reference to this object's data cast as a double. If This metadata is 
 * not a double type then an exception is thrown. 
 *
 * @return Reference to this object's double data. 
 */
double& EMetadata::toDouble()
{
   EDEBUG_FUNC(this)

   checkType(Double);
   return *static_cast<double*>(_data);
}






/*!
 * Returns a reference to this object's data cast as a string. If This metadata is 
 * not a string type then an exception is thrown. 
 *
 * @return Reference to this object's string data. 
 */
QString& EMetadata::toString()
{
   EDEBUG_FUNC(this)

   checkType(String);
   return *static_cast<QString*>(_data);
}






/*!
 * Returns a reference to this object's data cast as a byte array. If This metadata 
 * is not a byte type then an exception is thrown. 
 *
 * @return Reference to this object's byte array data. 
 */
QByteArray& EMetadata::toBytes()
{
   EDEBUG_FUNC(this)

   checkType(Bytes);
   return *static_cast<QByteArray*>(_data);
}






/*!
 * Returns a reference to this object's data cast as a meta array. If This metadata 
 * is not an array type then an exception is thrown. 
 *
 * @return Reference to this object's meta array. 
 */
EMetaArray& EMetadata::toArray()
{
   EDEBUG_FUNC(this)

   checkType(Array);
   return *static_cast<EMetaArray*>(_data);
}






/*!
 * Returns a reference to this object's data cast as a meta object. If This 
 * metadata is not an object type then an exception is thrown. 
 *
 * @return Reference to this object's meta object. 
 */
EMetaObject& EMetadata::toObject()
{
   EDEBUG_FUNC(this)

   checkType(Object);
   return *static_cast<EMetaObject*>(_data);
}






/*!
 * Returns the name of the given type as a string. 
 *
 * @param type Type to be given name of. 
 *
 * @return Name of the given type. 
 */
QString EMetadata::typeName(Type type)
{
   EDEBUG_FUNC()

   // Return constant string of correct type name from given type. 
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
 */
void EMetadata::checkType(Type type) const
{
   EDEBUG_FUNC(this,type)

   // If the given type does not match with this object's type then throw an 
   // exception, else return from operation. 
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
 */
void EMetadata::clear()
{
   EDEBUG_FUNC(this)

   // Delete the underlying data the data pointer points to, if any. 
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
 * Creates new data for this object. Overwrites any previous data pointer and does 
 * not free anything it pointed to. 
 */
void EMetadata::create()
{
   EDEBUG_FUNC(this)

   // Create new data based off the type of this object, setting this object's data 
   // pointer to the new data. 
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
 * Copy data from the supplied data pointer to the object's data pointer based off 
 * this object's type. If the type is Null then this object's data pointer is set 
 * to Null. 
 *
 * @param data The data pointer to use for possible copying. 
 */
void EMetadata::copy(const void* data)
{
   EDEBUG_FUNC(this,data)

   // Copy the data pointed to by this object's data pointer from a data pointed to 
   // by the supplied data pointer based of this object's type. If the type is Null 
   // then the data pointer is set to null and the supplied data pointer is ignored. 
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
