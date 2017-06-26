#include <QImage>

#include "metadata.h"
#include "exception.h"
#include "datastream.h"






Ace::Metadata::Metadata(Type type):
   _type(type)
{
   initialize(type);
}






Ace::Metadata::~Metadata()
{
   clear();
}






bool Ace::Metadata::isNull() const
{
   return _type == Null;
}






bool Ace::Metadata::isBool() const
{
   return _type == Bool;
}






bool Ace::Metadata::isDouble() const
{
   return _type == Double;
}






bool Ace::Metadata::isString() const
{
   return _type == String;
}






bool Ace::Metadata::isBytes() const
{
   return _type == Bytes;
}






bool Ace::Metadata::isArray() const
{
   return _type == Array;
}






bool Ace::Metadata::isObject() const
{
   return _type == Object;
}






const bool& Ace::Metadata::toBool() const
{
   return toType<bool>(Bool);
}






bool& Ace::Metadata::toBool()
{
   return toType<bool>(Bool);
}






const double& Ace::Metadata::toDouble() const
{
   return toType<double>(Double);
}






double& Ace::Metadata::toDouble()
{
   return toType<double>(Double);
}






const QString& Ace::Metadata::toString() const
{
   return toType<QString>(String);
}






QString& Ace::Metadata::toString()
{
   return toType<QString>(String);
}






const QByteArray& Ace::Metadata::toBytes() const
{
   return toType<QByteArray>(Bytes);
}






QByteArray& Ace::Metadata::toBytes()
{
   return toType<QByteArray>(Bytes);
}






const Ace::Metadata::List& Ace::Metadata::toArray() const
{
   return toType<List>(Array);
}






Ace::Metadata::List& Ace::Metadata::toArray()
{
   return toType<List>(Array);
}






const Ace::Metadata::Map& Ace::Metadata::toObject() const
{
   return toType<Map>(Object);
}






Ace::Metadata::Map& Ace::Metadata::toObject()
{
   return toType<Map>(Object);
}






QVariant Ace::Metadata::toVariant() const
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






void Ace::Metadata::setType(quint8 newType)
{
   // clear any existing data
   clear();

   // set new type and initialize
   _type = newType;
   initialize(newType);
}






quint8 Ace::Metadata::getType() const
{
   return _type;
}






void Ace::Metadata::clear()
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






void Ace::Metadata::setParent(Ace::Metadata *parent)
{
   _parent = parent;
}






Ace::Metadata *Ace::Metadata::getParent() const
{
   return _parent;
}






int Ace::Metadata::getChildIndex(Metadata* child) const
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

   // if data type is not array or object then report exception
   else
   {
      E_MAKE_EXCEPTION(e);
      e.setLevel(EException::Critical);
      e.setType(TypeMismatch);
      e.setTitle(QObject::tr("Metadata Type Mismatch"));
      e.setDetails(QObject::tr("Attempting to access children of metadata object that is of type"
                               " %1.")
                   .arg(convertTypeName(_type)));
      throw e;
   }

   // return invalid index if no child is found
   return -1;
}






QString Ace::Metadata::getTypeName() const
{
   return convertTypeName(_type);
}






void Ace::Metadata::initialize(quint8 type)
{
   // initialize new data depending on type
   switch (type)
   {
   case Bool:
      _data = reinterpret_cast<void*>(new bool);
      toBool() = false;
      break;
   case Double:
      _data = reinterpret_cast<void*>(new double);
      toDouble() = 0.0;
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
T& Ace::Metadata::toType(quint8 type)
{
   // make sure type is correct
   if ( _type != type )
   {
      E_MAKE_EXCEPTION(e);
      e.setLevel(EException::Critical);
      e.setType(TypeMismatch);
      e.setTitle(QObject::tr("Metadata Type Mismatch"));
      e.setDetails(QObject::tr("Attempting to use metadata value of type %1 as type %2.")
                   .arg(convertTypeName(_type)).arg(convertTypeName(type)));
      throw e;
   }

   // return reference to data
   return *reinterpret_cast<T*>(_data);
}






template<class T>
const T& Ace::Metadata::toType(quint8 type) const
{
   // make sure type is correct
    if ( _type != type )
    {
       E_MAKE_EXCEPTION(e);
       e.setLevel(EException::Critical);
       e.setType(TypeMismatch);
       e.setTitle(QObject::tr("Metadata Type Mismatch"));
       e.setDetails(QObject::tr("Attempting to use metadata value of type %1 as type %2.")
                    .arg(convertTypeName(_type)).arg(convertTypeName(type)));
       throw e;
    }

    // return reference to data
    return *reinterpret_cast<T*>(_data);
}






QString Ace::Metadata::convertTypeName(quint8 type) const
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






EDataStream& Ace::operator>>(EDataStream& stream, Ace::Metadata& meta)
{
   // clear the metadata, read new type, and initialize
   meta.clear();
   stream >> meta._type;
   meta.initialize(meta._type);

   // read the metadata value from stream depending on what type it is
   switch (meta._type)
   {
   case Metadata::Bool:
      stream >> meta.toBool();
      break;
   case Metadata::Double:
      stream >> meta.toDouble();
      break;
   case Metadata::String:
      stream >> meta.toString();
      break;
   case Metadata::Bytes:
      stream >> meta.toBytes();
      break;
   case Metadata::Array:
   {
      // if this is an array, get the array and read in the size
      Metadata::List& list {meta.toArray()};
      quint32 size;
      stream >> size;

      // go through total size adding new metadata objects to list and reading their values
      for (quint32 i = 0; i < size ;++i)
      {
         list.push_back(new Metadata);
         stream >> *(list.back());
      }
      break;
   }
   case Metadata::Object:
   {
      // if this is an object, get the map and read in the size
      Metadata::Map& map {meta.toObject()};
      quint32 size;
      stream >> size;

      // iterate through the total size
      for (quint32 i = 0; i < size ;++i)
      {
         // initialize key and metadata variables
         QString key;
         Metadata* meta {new Metadata};

         // read in key value and metadata and insert into map
         stream >> key >> *meta;
         map.insert(key,meta);
      }
      break;
   }
   case Metadata::Null:
      break;
   }

   // make sure the stream encountered no errors
   if ( !stream )
   {
      E_MAKE_EXCEPTION(e);
      e.setLevel(EException::Critical);
      e.setType(Metadata::CannotRead);
      e.setTitle(QObject::tr("Metadata"));
      e.setDetails(QObject::tr("Failed reading metadata."));
      throw e;
   }

   // return reference ot stream object
   return stream;
}







EDataStream& Ace::operator<<(EDataStream& stream, Ace::Metadata& meta)
{
   // write out metadata type
   stream << meta._type;

   // write out metadata value depending on what type it is
   switch (meta._type)
   {
   case Metadata::Bool:
      stream << meta.toBool();
      break;
   case Metadata::Double:
      stream << meta.toDouble();
      break;
   case Metadata::String:
      stream << meta.toString();
      break;
   case Metadata::Bytes:
      //stream << meta.toBytes();
      break;
   case Metadata::Array:
   {
      // if this is an array get list and write out size
      const Metadata::List& list {meta.toArray()};
      quint32 size = list.size();
      stream << size;

      // iterate through list and write out each metadata object
      for (auto i = list.constBegin(); i != list.constEnd() ;++i)
      {
         stream << **i;
      }
      break;
   }
   case Metadata::Object:
   {
      // if this is an object get map and write out size
      const Metadata::Map& map {meta.toObject()};
      quint32 size = map.size();
      stream << size;

      // iterate through map and write out each key and metadata object
      for (auto i = map.constBegin(); i != map.constEnd() ;++i)
      {
         stream << i.key() << **i;
      }
      break;
   }
   case Metadata::Null:
      break;
   }

   // make sure stream encountered no errors
   if ( !stream )
   {
      E_MAKE_EXCEPTION(e);
      e.setLevel(EException::Critical);
      e.setType(Metadata::CannotRead);
      e.setTitle(QObject::tr("Metadata"));
      e.setDetails(QObject::tr("Failed writing metadata."));
      throw e;
   }

   // return reference to stream
   return stream;
}
