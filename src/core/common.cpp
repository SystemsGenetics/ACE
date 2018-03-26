#include "common.h"
#include "edatastream.h"
#include "emetadata.h"
#include "emetaarray.h"
#include "emetaobject.h"
//






/*!
 * This reads in a metadata value from a data stream object. If the metadata is an 
 * array or object this recursively reads in all metadata contained therein. 
 *
 * @param stream The data stream to be read from. 
 *
 * @param meta The metadata object that will be overwritten with the metadata read 
 *             from the data stream. 
 *
 * @return Constant reference to data stream object. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Read in the metadata type and overwrite the given metadata with a new object 
 *    of that type. 
 *
 * 2. If the type is an array go to step 4, else if the type is an object go to 
 *    step 5, else go to step 3. 
 *
 * 3. Read in the value of the metadata to the given object. Return a reference to 
 *    the data stream. 
 *
 * 4. Read in the size of metadata objects the array contains in the data stream. 
 *    Read in all metadata objects the array contains, inserting each one into the 
 *    given metadata array object. Return a reference to the data stream. 
 *
 * 5. Read in the size of key and metadata value pairs the object contains in the 
 *    data stream. Read in all keys and metadata objects the object contains, 
 *    inserting each key and metadata value pair into the object. Return a 
 *    reference to the data stream. 
 */
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
   case EMetadata::Array:
      {
         quint32 size;
         stream >> size;
         for (quint32 i = 0; i < size ;++i)
         {
            EMetadata child;
            stream >> child;
            meta.toArray().append(child);
         }
         break;
      }
   case EMetadata::Object:
      {
         quint32 size;
         stream >> size;
         for (quint32 i = 0; i < size ;++i)
         {
            QString key;
            EMetadata child;
            stream >> key >> child;
            meta.toObject().insert(key,child);
         }
         break;
      }
   default: break;
   }
   return stream;
}






/*!
 * This writes out a metadata value to the given data stream from the metadata 
 * object given. If the given metadata object is an array or object this 
 * recursively writes out all metadata contained therein. 
 *
 * @param stream The data stream object to be written to. 
 *
 * @param meta The metadata object whose value will be written to the data stream. 
 *
 * @return Reference to data stream object. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Write out the metadata type from the given object to the data stream. 
 *
 * 2. If the type is an array go to step 4, else if the type is an object go to 
 *    step 5, else go to step 3. 
 *
 * 3. Write out the value of the given metadata object to the data stream. 
 *
 * 4. Write out the number of metadata objects the array contains. Write out all 
 *    metadata values the array contains to the data stream. Return a reference to 
 *    the data stream. 
 *
 * 5. Write out the number of key and metadata value pairs the object contains. 
 *    Write out all key and metadata value pairs the object contains to the data 
 *    stream. Return a reference to the data stream. 
 */
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
   case EMetadata::Array:
      stream << static_cast<quint32>(meta.toArray().size());
      for (auto child : meta.toArray())
      {
         stream << child;
      }
      break;
   case EMetadata::Object:
      stream << static_cast<quint32>(meta.toObject().size());
      for (auto i = meta.toObject().begin(); i != meta.toObject().end() ;++i)
      {
         stream << i.key() << *i;
      }
      break;
   default: break;
   }
   return stream;
}
