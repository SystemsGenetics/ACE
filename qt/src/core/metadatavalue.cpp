#include <QImage>

#include "metadatavalue.h"
#include "exception.h"






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






bool Ace::Metadata::isImage() const
{
   return _type == Image;
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






const QImage& Ace::Metadata::toImage() const
{
   return toType<QImage>(Image);
}






QImage& Ace::Metadata::toImage()
{
   return toType<QImage>(Image);
}






const QList<Ace::Metadata*>& Ace::Metadata::toArray() const
{
   return toType<QList<Metadata*>>(Array);
}






QList<Ace::Metadata*>& Ace::Metadata::toArray()
{
   return toType<QList<Metadata*>>(Array);
}






const QMap<QString,Ace::Metadata*>& Ace::Metadata::toObject() const
{
   return toType<QMap<QString,Metadata*>>(Object);
}






QMap<QString,Ace::Metadata*>& Ace::Metadata::toObject()
{
   return toType<QMap<QString,Metadata*>>(Object);
}






void Ace::Metadata::setType(Type newType)
{
   clear();
   _type = newType;
   initialize(newType);
}






Ace::Metadata::Type Ace::Metadata::getType() const
{
   return _type;
}






void Ace::Metadata::clear()
{
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
   case Image:
      delete reinterpret_cast<QImage*>(_data);
      break;
   case Array:
   {
      auto data = reinterpret_cast<QList<Metadata*>*>(_data);
      qDeleteAll(*data);
      delete data;
      break;
   }
   case Object:
   {
      auto data = reinterpret_cast<QMap<QString,Metadata*>*>(_data);
      qDeleteAll(*data);
      delete data;
      break;
   }
   case Null:
      break;
   }
   _type = Null;
   _data = nullptr;
}






void Ace::Metadata::initialize(Type type)
{
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
   case Image:
      _data = reinterpret_cast<void*>(new QImage);
      break;
   case Array:
      _data = reinterpret_cast<void*>(new QList<Metadata*>);
      break;
   case Object:
      _data = reinterpret_cast<void*>(new QMap<QString,Metadata*>);
      break;
   case Null:
      break;
   }
}






template<class T>
T& Ace::Metadata::toType(Type type)
{
   if ( _type != type )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Metadata Type Mismatch"));
      e.setDetails(QObject::tr("Attempting to use metadata value of type %1 as type %2.")
                   .arg(getTypeName(_type)).arg(getTypeName(type)));
      throw e;
   }
   return *reinterpret_cast<T*>(_data);
}






template<class T>
const T& Ace::Metadata::toType(Type type) const
{
    if ( _type != type )
    {
       E_MAKE_EXCEPTION(e);
       e.setTitle(QObject::tr("Metadata Type Mismatch"));
       e.setDetails(QObject::tr("Attempting to use metadata value of type %1 as type %2.")
                    .arg(getTypeName(_type)).arg(getTypeName(type)));
       throw e;
    }
    return *reinterpret_cast<T*>(_data);
}






QString Ace::Metadata::getTypeName(Ace::Metadata::Type type) const
{
   switch (type)
   {
   case Bool:
      return QString("Bool");
   case Double:
      return QString("Double");
   case String:
      return QString("String");
   case Image:
      return QString("Image");
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
