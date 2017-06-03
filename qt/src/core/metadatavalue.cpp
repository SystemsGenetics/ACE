#include <QImage>

#include "metadatavalue.h"
#include "metadataarray.h"
#include "metadataobject.h"
#include "exception.h"






Ace::MetadataValue::MetadataValue(Type type):
   _type(type)
{
   initialize(type);
}






Ace::MetadataValue::~MetadataValue()
{
   clear();
}






Ace::MetadataValue::MetadataValue(const MetadataValue& copy)
{
   copy.checkCopyOrMove("copy");
   _type = copy._type;
   initialize(copy._type);
   makeCopy(copy);
}






Ace::MetadataValue::MetadataValue(Ace::MetadataValue&& move)
{
   move.checkCopyOrMove("move");
   _type = move._type;
   _data = move._data;
   move._type = Null;
   move._data = nullptr;
}






Ace::MetadataValue& Ace::MetadataValue::operator=(const MetadataValue& copy)
{
   copy.checkCopyOrMove("copy");
   setType(copy._type);
   makeCopy(copy);
   return *this;
}






Ace::MetadataValue& Ace::MetadataValue::operator=(MetadataValue&& move)
{
   move.checkCopyOrMove("move");
   clear();
   _type = move._type;
   _data = move._data;
   move._type = Null;
   move._data = nullptr;
   return *this;
}






bool Ace::MetadataValue::isNull() const
{
   return _type == Null;
}






bool Ace::MetadataValue::isBool() const
{
   return _type == Bool;
}






bool Ace::MetadataValue::isDouble() const
{
   return _type == Double;
}






bool Ace::MetadataValue::isString() const
{
   return _type == String;
}






bool Ace::MetadataValue::isImage() const
{
   return _type == Image;
}






bool Ace::MetadataValue::isArray() const
{
   return _type == Array;
}






bool Ace::MetadataValue::isObject() const
{
   return _type == Object;
}






const bool& Ace::MetadataValue::toBool() const
{
   checkType(Bool);
   return *reinterpret_cast<bool*>(_data);
}






bool& Ace::MetadataValue::toBool()
{
   checkType(Bool);
   return *reinterpret_cast<bool*>(_data);
}






const double& Ace::MetadataValue::toDouble() const
{
   checkType(Double);
   return *reinterpret_cast<double*>(_data);
}






double& Ace::MetadataValue::toDouble()
{
   checkType(Double);
   return *reinterpret_cast<double*>(_data);
}






const QString& Ace::MetadataValue::toString() const
{
   checkType(String);
   return *reinterpret_cast<QString*>(_data);
}






QString& Ace::MetadataValue::toString()
{
   checkType(String);
   return *reinterpret_cast<QString*>(_data);
}






const QImage& Ace::MetadataValue::toImage() const
{
   checkType(Image);
   return *reinterpret_cast<QImage*>(_data);
}






QImage& Ace::MetadataValue::toImage()
{
   checkType(Image);
   return *reinterpret_cast<QImage*>(_data);
}






const Ace::MetadataArray& Ace::MetadataValue::toArray() const
{
   checkType(Array);
   return *reinterpret_cast<MetadataArray*>(_data);
}






Ace::MetadataArray& Ace::MetadataValue::toArray()
{
   checkType(Array);
   return *reinterpret_cast<MetadataArray*>(_data);
}






const Ace::MetadataObject& Ace::MetadataValue::toObject() const
{
   checkType(Object);
   return *reinterpret_cast<MetadataObject*>(_data);
}






Ace::MetadataObject& Ace::MetadataValue::toObject()
{
   checkType(Object);
   return *reinterpret_cast<MetadataObject*>(_data);
}






void Ace::MetadataValue::setType(Type newType)
{
   clear();
   _type = newType;
   initialize(newType);
}






Ace::MetadataValue::Type Ace::MetadataValue::getType() const
{
   return _type;
}






void Ace::MetadataValue::clear()
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
      delete reinterpret_cast<MetadataArray*>(_data);
      break;
   case Object:
      delete reinterpret_cast<MetadataObject*>(_data);
      break;
   case Null:
      break;
   }
   _type = Null;
   _data = nullptr;
}






void Ace::MetadataValue::initialize(Type type)
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
      _data = reinterpret_cast<void*>(new MetadataArray);
      break;
   case Object:
      _data = reinterpret_cast<void*>(new MetadataObject);
      break;
   case Null:
      break;
   }
}






void Ace::MetadataValue::checkType(Type type) const
{
   if ( _type != type )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Metadata Type Mismatch"));
      e.setDetails(QObject::tr("Attempting to use metadata value of type %1 as type %2.")
                   .arg(getTypeName(_type)).arg(getTypeName(type)));
      throw e;
   }
}






void Ace::MetadataValue::checkCopyOrMove(const QString& what) const
{
   if ( _type == Array || _type == Object )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid Metadata Copy"));
      e.setDetails(QObject::tr("Cannot %1 metadata array or object.").arg(what));
      throw e;
   }
}






void Ace::MetadataValue::makeCopy(const MetadataValue& copy)
{
   switch (_type)
   {
   case Bool:
      toBool() = copy.toBool();
      break;
   case Double:
      toDouble() = copy.toDouble();
      break;
   case String:
      toString() = copy.toString();
      break;
   case Image:
      toImage() = copy.toImage();
      break;
   case Array:
   case Object:
   case Null:
      break;
   }
}






QString Ace::MetadataValue::getTypeName(Ace::MetadataValue::Type type) const
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
