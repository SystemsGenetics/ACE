#include "common.h"
#include "datastream.h"
#include "emetadata.h"






const EDataStream& operator>>(const EDataStream& stream, EMetadata& meta)
{
   quint8 type;
   stream >> type;
   meta = EMetadata(static_cast<EMetadata::Type>(type));
   switch (meta.type())
   {
   case EMetadata::Bool:
   {
      quint8 val;
      stream >> val;
      meta.toBool() = val;
      break;
   }
   case EMetadata::Double:
      stream >> meta.toDouble();
      break;
   case EMetadata::String:
      stream >> meta.toString();
      break;
   case EMetadata::Bytes:
      stream >> meta.toBytes();
      break;
   default: break;
   }
   return stream;
}






EDataStream& operator<<(EDataStream& stream, const EMetadata& meta)
{
   quint8 type {static_cast<quint8>(meta.type())};
   stream << type;
   switch (meta.type())
   {
   case EMetadata::Bool:
      stream << meta.toBool();
      break;
   case EMetadata::Double:
      stream << meta.toDouble();
      break;
   case EMetadata::String:
      stream << meta.toString();
      break;
   case EMetadata::Bytes:
      stream << meta.toBytes();
      break;
   default: break;
   }
   return stream;
}
