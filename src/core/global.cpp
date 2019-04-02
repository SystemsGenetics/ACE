#include "global.h"
#include "edatastream.h"
#include "emetadata.h"
#include "emetaarray.h"
#include "emetaobject.h"
#include "edebug.h"






/*!
 * This reads in a metadata value from a data stream object. If the metadata is
 * an array or object this recursively reads in all metadata contained therein.
 *
 * @param stream The data stream to be read from.
 *
 * @param meta The metadata object that will be overwritten with the metadata
 *             read from the data stream.
 *
 * @return Constant reference to data stream object.
 */
const EDataStream& operator>>(const EDataStream& stream, EMetadata& meta)
{
   // Add the debug header.
   EDEBUG_FUNC(&stream,&meta);

   // Read in the metadata type.
   quint8 type;
   stream >> type;

   // Overwrite the given metadata with a new object of the read in type.
   meta = EMetadata(static_cast<EMetadata::Type>(type));

   // Determine which metadata type is being read in and then read in the appropriate
   // data.
   switch (meta.type())
   {
   case EMetadata::Bool:
   {
      // Read in the boolean value of the metadata as a 8 bit unsigned integer.
      quint8 val;
      stream >> val;

      // Overwrite the boolean value of the given metadata to the read in integer value.
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
         // Read in the size of metadata objects the array contains in the data stream.
         quint32 size;
         stream >> size;

         // Iterate through all metadata children to be read into the metadata array.
         for (quint32 i = 0; i < size ;++i)
         {
            // Read in the child metadata.
            EMetadata child;
            stream >> child;

            // Append it to the parent metadata array.
            meta.toArray().append(child);
         }
         break;
      }
   case EMetadata::Object:
      {
         // Read in the size of key and metadata value pairs the object contains in the
         // data stream.
         quint32 size;
         stream >> size;

         // Iterate through all child metadata objects to be read into the metadata object.
         for (quint32 i = 0; i < size ;++i)
         {
            // Create an empty key string and metadata object used to read in the next child.
            QString key;
            EMetadata child;

            // Read in the next key and child metadata.
            stream >> key >> child;

            // Insert the read in key and child metadata to the parent metadata object.
            meta.toObject().insert(key,child);
         }
         break;
      }
   default: break;
   }

   // Return the given data stream.
   return stream;
}






/*!
 * This writes out a metadata value to the given data stream from the metadata
 * object given. If the given metadata object is an array or object this
 * recursively writes out all metadata contained therein.
 *
 * @param stream The data stream object to be written to.
 *
 * @param meta The metadata object whose value will be written to the data
 *             stream.
 *
 * @return Reference to data stream object.
 */
EDataStream& operator<<(EDataStream& stream, const EMetadata& meta)
{
   // Add the debug header.
   EDEBUG_FUNC(&stream,&meta);

   // Write out the metadata type from the given object to the given data stream.
   quint8 type {static_cast<quint8>(meta.type())};
   stream << type;

   // Determine which type the given metadata is and write out its data to the given
   // data stream cast to the appropriate type determined.
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

      // Write out the number of metadata objects the array contains.
      stream << static_cast<quint32>(meta.toArray().size());

      // Iterate through all metadata values the array contains and write them to the
      // data stream.
      for (auto child : meta.toArray())
      {
         stream << child;
      }
      break;
   case EMetadata::Object:

      // Write out the number of key and metadata value pairs the object contains.
      stream << static_cast<quint32>(meta.toObject().size());

      // Iterate through all key and metadata value pairs the object contains and write
      // the pairs to the data stream.
      for (auto i = meta.toObject().begin(); i != meta.toObject().end() ;++i)
      {
         stream << i.key() << *i;
      }
      break;
   default: break;
   }

   // Return a reference to the given data stream.
   return stream;
}






/*!
 * Writes a metadata object to the debug stream for debugging output.
 *
 * @param debug The debug object which has the given metadata object value
 *              streamed to it.
 *
 * @param meta The metadata value that is streamed to the given debug object.
 *
 * @return Reference to the debug object.
 */
EDebug& operator<<(EDebug& debug, const EMetadata*const meta)
{
   // Disable quoting of strings.
   debug << EDebug::NoQuote;

   // Append the fact that this is metadata and its type.
   switch (meta->_type)
   {
   case EMetadata::Null:
      debug << QStringLiteral("EMetadata(null");
      break;
   case EMetadata::Bool:
      debug << QStringLiteral("EMetadata(bool,");
      break;
   case EMetadata::Double:
      debug << QStringLiteral("EMetadata(double,");
      break;
   case EMetadata::String:
      debug << QStringLiteral("EMetadata(string,\"");
      break;
   case EMetadata::Bytes:
      debug << QStringLiteral("EMetadata(bytes");
      break;
   case EMetadata::Array:
      debug << QStringLiteral("EMetadata(array");
      break;
   case EMetadata::Object:
      debug << QStringLiteral("EMetadata(object");
      break;
   }

   // Check to see if this metadata object has data.
   if ( meta->_data )
   {
      // Append the value of this metadata's data as a string.
      switch (meta->_type)
      {
      case EMetadata::Bool:
         debug << ( *reinterpret_cast<bool*>(meta->_data) ? QStringLiteral("TRUE") : QStringLiteral("FALSE") )
               << QStringLiteral(")");
         break;
      case EMetadata::Double:
         debug << *reinterpret_cast<double*>(meta->_data)
               << QStringLiteral(")");
         break;
      case EMetadata::String:
         debug << *reinterpret_cast<QString*>(meta->_data)
               << QStringLiteral("\")");
         break;
      default:
         debug << QStringLiteral(")");
         break;
      }
   }

   // Else this must be a null type so just add a closing parenthesis.
   else
   {
      debug << QStringLiteral(")");
   }

   // Enable quoting of strings and return a reference to the debug object.
   debug << EDebug::Quote;
   return debug;
}






/*!
 * Writes a abstract analytic block object to the debug stream for debugging
 * output.
 *
 * @param debug The debug object which has the given abstract analytic block
 *              value streamed to it.
 *
 * @param value The abstract analytic object that is streamed to the given debug
 *              object.
 *
 * @return Reference to the debug object.
 */
EDebug& operator<<(EDebug& debug, const EAbstractAnalytic::Block*const value)
{
   // Append the raw pointer value to the given debug stream.
   debug << static_cast<const void*>(value);

   // If the given pointer is not null then append the fact it is a block and give
   // its index value.
   if ( value )
   {
      debug << EDebug::NoQuote << QStringLiteral("(EAbstractAnalytic::Block,index=")
            << QString::number(value->_index)
            << QStringLiteral(")")
            << EDebug::Quote;
   }

   // Return a reference to the debug object.
   return debug;
}

