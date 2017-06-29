#include <QImage>

#include "metadata.h"
#include "exception.h"
#include "datastream.h"






EMetadata::EMetadata(Type type):
   _type(type)
{
   initialize(type);
}






EMetadata::~EMetadata()
{
   clear();
}






bool EMetadata::isNull() const
{
   return _type == Null;
}






bool EMetadata::isBool() const
{
   return _type == Bool;
}






bool EMetadata::isDouble() const
{
   return _type == Double;
}






bool EMetadata::isString() const
{
   return _type == String;
}






bool EMetadata::isBytes() const
{
   return _type == Bytes;
}






bool EMetadata::isArray() const
{
   return _type == Array;
}






bool EMetadata::isObject() const
{
   return _type == Object;
}






const bool* EMetadata::toBool() const
{
   return toType<bool>(Bool);
}






bool* EMetadata::toBool()
{
   return toType<bool>(Bool);
}






const double* EMetadata::toDouble() const
{
   return toType<double>(Double);
}






double* EMetadata::toDouble()
{
   return toType<double>(Double);
}






const QString* EMetadata::toString() const
{
   return toType<QString>(String);
}






QString* EMetadata::toString()
{
   return toType<QString>(String);
}






const QByteArray* EMetadata::toBytes() const
{
   return toType<QByteArray>(Bytes);
}






QByteArray* EMetadata::toBytes()
{
   return toType<QByteArray>(Bytes);
}






const EMetadata::List* EMetadata::toArray() const
{
   return toType<List>(Array);
}






EMetadata::List* EMetadata::toArray()
{
   return toType<List>(Array);
}






const EMetadata::Map* EMetadata::toObject() const
{
   return toType<Map>(Object);
}






EMetadata::Map* EMetadata::toObject()
{
   return toType<Map>(Object);
}






QVariant EMetadata::toVariant() const
{
   // figure out which type and return data wrapped in qt variant
   switch (_type)
   {
   case Bool:
      QVariant(*reinterpret_cast<bool*>(_data));
      break;
   case Double:
      QVariant(*reinterpret_cast<double*>(_data));
      break;
   case String:
      QVariant(*reinterpret_cast<QString*>(_data));
      break;
   case Bytes:
      // if type is bytes simply state it is a byte array
      QVariant(QObject::tr("Byte Array"));
      break;
   case Array:
   case Object:
   case Null:
      // if type is array, object, or null return nothing
      return QVariant();
   }
   return QVariant();
}






void EMetadata::setType(quint8 newType)
{
   // clear any existing data
   clear();

   // set new type and initialize
   _type = newType;
   initialize(newType);
}






quint8 EMetadata::getType() const
{
   return _type;
}






void EMetadata::clear()
{
   // delete data depending on which type
   switch (_type)
   {
   case Bool:
      delete reinterpret_cast<bool*>(_data);
      break;
   case Double:
      delete reinterpret_cast<double*>(_data);
      break;
   case String:
      delete reinterpret_cast<QString*>(_data);
      break;
   case Bytes:
      delete reinterpret_cast<QByteArray*>(_data);
      break;
   case Array:
   {
      // delete all array children
      auto data = reinterpret_cast<List*>(_data);
      qDeleteAll(*data);

      // delete array itself
      delete data;
      break;
   }
   case Object:
   {
      // delete all object children
      auto data = reinterpret_cast<Map*>(_data);
      for (auto i = data->constBegin(); i != data->constEnd() ;++i)
      {
          delete *i;
      }

      // delete object itself
      delete data;
      break;
   }
   case Null:
      // null data, do nothing
      break;
   }

   // set type to null
   _type = Null;
   _data = nullptr;
}






void EMetadata::setParent(EMetadata *parent)
{
   _parent = parent;
}






EMetadata *EMetadata::getParent() const
{
   return _parent;
}






int EMetadata::getChildIndex(EMetadata* child) const
{
   // check if data type is array
   if ( _type == Array )
   {
      // get array and iterate through it until child pointer is found
      List& list {*reinterpret_cast<List*>(_data)};
      return list.lastIndexOf(child);
   }

   // check if data type is object
   else if ( _type == Object )
   {
      // get map and extract list of values from it
      Map& map {*reinterpret_cast<Map*>(_data)};
      List list = map.values();

      // iterate through list of values until child pointer is found
      for (int i = 0; i < list.size() ;++i)
      {
         if ( list.at(i) == child )
         {
            return i;
         }
      }
   }

   // if data type is not array or object then error
   else
   {
      return -1;
   }

   // return invalid index if no child is found
   return -1;
}






QString EMetadata::getTypeName() const
{
   return convertTypeName(_type);
}






void EMetadata::initialize(quint8 type)
{
   // initialize new data depending on type
   switch (type)
   {
   case Bool:
      _data = reinterpret_cast<void*>(new bool);
      *(toBool()) = false;
      break;
   case Double:
      _data = reinterpret_cast<void*>(new double);
      *(toDouble()) = 0.0;
      break;
   case String:
      _data = reinterpret_cast<void*>(new QString);
      break;
   case Bytes:
      _data = reinterpret_cast<void*>(new QByteArray);
      break;
   case Array:
      _data = reinterpret_cast<void*>(new List);
      break;
   case Object:
      _data = reinterpret_cast<void*>(new Map);
      break;
   case Null:
      // null data, nothing to initialize
      break;
   }
}






template<class T>
T* EMetadata::toType(quint8 type)
{
   // make sure type is correct
   if ( _type != type )
   {
      return nullptr;
   }

   // return reference to data
   return reinterpret_cast<T*>(_data);
}






template<class T>
const T* EMetadata::toType(quint8 type) const
{
   // make sure type is correct
    if ( _type != type )
    {
       return nullptr;
    }

    // return reference to data
    return reinterpret_cast<T*>(_data);
}






QString EMetadata::convertTypeName(quint8 type) const
{
   // determine which type object is and return name
   switch (type)
   {
   case Bool:
      return QString("Bool");
   case Double:
      return QString("Double");
   case String:
      return QString("String");
   case Bytes:
      return QString("Bytes");
   case Array:
      return QString("Array");
   case Object:
      return QString("Object");
   case Null:
      return QString("Null");
   default:
      return QString();
   }
}






EDataStream& operator>>(EDataStream& stream, EMetadata& meta)
{
   // clear the EMetadata, read new type, and initialize
   meta.clear();
   stream >> meta._type;
   meta.initialize(meta._type);

   // read the EMetadata value from stream depending on what type it is
   switch (meta._type)
   {
   case EMetadata::Bool:
      stream >> *(meta.toBool());
      break;
   case EMetadata::Double:
      stream >> *(meta.toDouble());
      break;
   case EMetadata::String:
      stream >> *(meta.toString());
      break;
   case EMetadata::Bytes:
      stream >> *(meta.toBytes());
      break;
   case EMetadata::Array:
   {
      // if this is an array, get the array and read in the size
      EMetadata::List& list {*(meta.toArray())};
      quint32 size;
      stream >> size;

      // go through total size adding new EMetadata objects to list and reading their values
      for (quint32 i = 0; i < size ;++i)
      {
         list.push_back(new EMetadata);
         stream >> *(list.back());
      }
      break;
   }
   case EMetadata::Object:
   {
      // if this is an object, get the map and read in the size
      EMetadata::Map& map {*(meta.toObject())};
      quint32 size;
      stream >> size;

      // iterate through the total size
      for (quint32 i = 0; i < size ;++i)
      {
         // initialize key and EMetadata variables
         QString key;
         EMetadata* meta {new EMetadata};

         // read in key value and EMetadata and insert into map
         stream >> key >> *meta;
         map.insert(key,meta);
      }
      break;
   }
   case EMetadata::Null:
      break;
   }

   // return reference ot stream object
   return stream;
}







EDataStream& operator<<(EDataStream& stream, EMetadata& meta)
{
   // write out EMetadata type
   stream << meta._type;

   // write out EMetadata value depending on what type it is
   switch (meta._type)
   {
   case EMetadata::Bool:
      stream << *(meta.toBool());
      break;
   case EMetadata::Double:
      stream << *(meta.toDouble());
      break;
   case EMetadata::String:
      stream << *(meta.toString());
      break;
   case EMetadata::Bytes:
      stream << *(meta.toBytes());
      break;
   case EMetadata::Array:
   {
      // if this is an array get list and write out size
      const EMetadata::List& list {*(meta.toArray())};
      quint32 size = list.size();
      stream << size;

      // iterate through list and write out each EMetadata object
      for (auto i = list.constBegin(); i != list.constEnd() ;++i)
      {
         stream << **i;
      }
      break;
   }
   case EMetadata::Object:
   {
      // if this is an object get map and write out size
      const EMetadata::Map& map {*(meta.toObject())};
      quint32 size = map.size();
      stream << size;

      // iterate through map and write out each key and EMetadata object
      for (auto i = map.constBegin(); i != map.constEnd() ;++i)
      {
         stream << i.key() << **i;
      }
      break;
   }
   case EMetadata::Null:
      break;
   }

   // return reference to stream
   return stream;
}
