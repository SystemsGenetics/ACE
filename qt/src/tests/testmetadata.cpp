#include "testmetadata.h"
#include "metadata.h"
#include "datastream.h"






void TestMetadata::testBool()
{
   // initialize metadata
   EMetadata data1(EMetadata::Bool);
   EMetadata data2(EMetadata::Null);

   // test first bool metadata
   Q_ASSERT( data1.getType() == EMetadata::Bool );
   Q_ASSERT( data1.isBool() );
   Q_ASSERT( !data1.isDouble() );
   Q_ASSERT( !data1.isString() );
   Q_ASSERT( !data1.isBytes() );
   Q_ASSERT( !data1.isArray() );
   Q_ASSERT( !data1.isObject() );
   Q_ASSERT( !data1.isNull() );
   *(data1.toBool()) = true;
   Q_ASSERT( *(data1.toBool()) );

   // test second bool metadata
   data2.setType(EMetadata::Bool);
   Q_ASSERT( data2.getType() == EMetadata::Bool );
   Q_ASSERT( data2.isBool() );
   Q_ASSERT( !data2.isDouble() );
   Q_ASSERT( !data2.isString() );
   Q_ASSERT( !data2.isBytes() );
   Q_ASSERT( !data2.isArray() );
   Q_ASSERT( !data2.isObject() );
   Q_ASSERT( !data2.isNull() );
   *(data2.toBool()) = true;
   Q_ASSERT( *(data2.toBool()) );
}






void TestMetadata::testDouble()
{
   // initialize metadata
   EMetadata data1(EMetadata::Double);
   EMetadata data2(EMetadata::Null);

   // test first double metadata
   Q_ASSERT( data1.getType() == EMetadata::Double );
   Q_ASSERT( !data1.isBool() );
   Q_ASSERT( data1.isDouble() );
   Q_ASSERT( !data1.isString() );
   Q_ASSERT( !data1.isBytes() );
   Q_ASSERT( !data1.isArray() );
   Q_ASSERT( !data1.isObject() );
   Q_ASSERT( !data1.isNull() );
   *(data1.toDouble()) = 3.14;
   Q_ASSERT( *(data1.toDouble()) == 3.14 );

   // test second double metadata
   data2.setType(EMetadata::Double);
   Q_ASSERT( data2.getType() == EMetadata::Double );
   Q_ASSERT( !data2.isBool() );
   Q_ASSERT( data2.isDouble() );
   Q_ASSERT( !data2.isString() );
   Q_ASSERT( !data2.isBytes() );
   Q_ASSERT( !data2.isArray() );
   Q_ASSERT( !data2.isObject() );
   Q_ASSERT( !data2.isNull() );
   *(data2.toDouble()) = 3.14;
   Q_ASSERT( *(data2.toDouble()) == 3.14 );
}






void TestMetadata::testString()
{
   // initialize metadata
   EMetadata data1(EMetadata::String);
   EMetadata data2(EMetadata::Null);

   // test first string metadata
   Q_ASSERT( data1.getType() == EMetadata::String );
   Q_ASSERT( !data1.isBool() );
   Q_ASSERT( !data1.isDouble() );
   Q_ASSERT( data1.isString() );
   Q_ASSERT( !data1.isBytes() );
   Q_ASSERT( !data1.isArray() );
   Q_ASSERT( !data1.isObject() );
   Q_ASSERT( !data1.isNull() );
   *(data1.toString()) = QString("test string");
   Q_ASSERT( *(data1.toString()) == QString("test string") );

   // test second string metadata
   data2.setType(EMetadata::String);
   Q_ASSERT( data2.getType() == EMetadata::String );
   Q_ASSERT( !data2.isBool() );
   Q_ASSERT( !data2.isDouble() );
   Q_ASSERT( data2.isString() );
   Q_ASSERT( !data2.isBytes() );
   Q_ASSERT( !data2.isArray() );
   Q_ASSERT( !data2.isObject() );
   Q_ASSERT( !data2.isNull() );
   *(data2.toString()) = QString("test string");
   Q_ASSERT( *(data2.toString()) == QString("test string") );
}






void TestMetadata::testBytes()
{
   // initialize metadata
   EMetadata data1(EMetadata::Bytes);
   EMetadata data2(EMetadata::Null);

   // test first bytes metadata
   Q_ASSERT( data1.getType() == EMetadata::Bytes );
   Q_ASSERT( !data1.isBool() );
   Q_ASSERT( !data1.isDouble() );
   Q_ASSERT( !data1.isString() );
   Q_ASSERT( data1.isBytes() );
   Q_ASSERT( !data1.isArray() );
   Q_ASSERT( !data1.isObject() );
   Q_ASSERT( !data1.isNull() );
   *(data1.toBytes()) = QByteArray("test bytes");
   Q_ASSERT( *(data1.toBytes()) == QByteArray("test bytes") );

   // test second bytes metadata
   data2.setType(EMetadata::Bytes);
   Q_ASSERT( data2.getType() == EMetadata::Bytes );
   Q_ASSERT( !data2.isBool() );
   Q_ASSERT( !data2.isDouble() );
   Q_ASSERT( !data2.isString() );
   Q_ASSERT( data2.isBytes() );
   Q_ASSERT( !data2.isArray() );
   Q_ASSERT( !data2.isObject() );
   Q_ASSERT( !data2.isNull() );
   *(data2.toBytes()) = QByteArray("test bytes");
   Q_ASSERT( *(data2.toBytes()) == QByteArray("test bytes") );
}






void TestMetadata::testArray()
{
   // initialize metadata
   EMetadata data1(EMetadata::Array);
   EMetadata data2(EMetadata::Null);

   // test first array metadata
   Q_ASSERT( data1.getType() == EMetadata::Array );
   Q_ASSERT( !data1.isBool() );
   Q_ASSERT( !data1.isDouble() );
   Q_ASSERT( !data1.isString() );
   Q_ASSERT( !data1.isBytes() );
   Q_ASSERT( data1.isArray() );
   Q_ASSERT( !data1.isObject() );
   Q_ASSERT( !data1.isNull() );
   data1.toArray()->append(new EMetadata);
   Q_ASSERT( data1.toArray()->size() == 1 );

   // test second array metadata
   data2.setType(EMetadata::Array);
   Q_ASSERT( data2.getType() == EMetadata::Array );
   Q_ASSERT( !data2.isBool() );
   Q_ASSERT( !data2.isDouble() );
   Q_ASSERT( !data2.isString() );
   Q_ASSERT( !data2.isBytes() );
   Q_ASSERT( data2.isArray() );
   Q_ASSERT( !data2.isObject() );
   Q_ASSERT( !data2.isNull() );
   data2.toArray()->append(new EMetadata);
   Q_ASSERT( data2.toArray()->size() == 1 );
}






void TestMetadata::testObject()
{
   // initialize metadata
   EMetadata data1(EMetadata::Object);
   EMetadata data2(EMetadata::Null);

   // test first object metadata
   Q_ASSERT( data1.getType() == EMetadata::Object );
   Q_ASSERT( !data1.isBool() );
   Q_ASSERT( !data1.isDouble() );
   Q_ASSERT( !data1.isString() );
   Q_ASSERT( !data1.isBytes() );
   Q_ASSERT( !data1.isArray() );
   Q_ASSERT( data1.isObject() );
   Q_ASSERT( !data1.isNull() );
   data1.toObject()->insert("test",new EMetadata);
   Q_ASSERT( data1.toObject()->size() == 1 );

   // test second object metadata
   data2.setType(EMetadata::Object);
   Q_ASSERT( data2.getType() == EMetadata::Object );
   Q_ASSERT( !data2.isBool() );
   Q_ASSERT( !data2.isDouble() );
   Q_ASSERT( !data2.isString() );
   Q_ASSERT( !data2.isBytes() );
   Q_ASSERT( !data2.isArray() );
   Q_ASSERT( data2.isObject() );
   Q_ASSERT( !data2.isNull() );
   data2.toObject()->insert("test",new EMetadata);
   Q_ASSERT( data2.toObject()->size() == 1 );
}






void TestMetadata::testNull()
{
   // initialize metadata
   EMetadata data1(EMetadata::Null);
   EMetadata data2(EMetadata::Bool);

   // test first null metadata
   Q_ASSERT( data1.getType() == EMetadata::Null );
   Q_ASSERT( !data1.isBool() );
   Q_ASSERT( !data1.isDouble() );
   Q_ASSERT( !data1.isString() );
   Q_ASSERT( !data1.isBytes() );
   Q_ASSERT( !data1.isArray() );
   Q_ASSERT( !data1.isObject() );
   Q_ASSERT( data1.isNull() );

   // test second null metadata
   data2.setType(EMetadata::Null);
   Q_ASSERT( data2.getType() == EMetadata::Null );
   Q_ASSERT( !data2.isBool() );
   Q_ASSERT( !data2.isDouble() );
   Q_ASSERT( !data2.isString() );
   Q_ASSERT( !data2.isBytes() );
   Q_ASSERT( !data2.isArray() );
   Q_ASSERT( !data2.isObject() );
   Q_ASSERT( data2.isNull() );
}






void TestMetadata::testFileInputOutput()
{
   // open file and initialize for stream
   QFile file(QDir::tempPath() + "/ACE_metadata_raw.io");
   Q_ASSERT( file.open(QIODevice::ReadWrite) );
   EDataStream* stream = new EDataStream(&file);

   // create metadata structure that will be written
   EMetadata in(EMetadata::Object);
   in.toObject()->insert("null",new EMetadata);
   in.toObject()->insert("bool",new EMetadata(EMetadata::Bool));
   *((*(in.toObject()))["bool"]->toBool()) = true;
   in.toObject()->insert("double",new EMetadata(EMetadata::Double));
   *((*(in.toObject()))["double"]->toDouble()) = 3.14;
   in.toObject()->insert("string",new EMetadata(EMetadata::String));
   *((*(in.toObject()))["string"]->toString()) = QString("test string");
   in.toObject()->insert("bytes",new EMetadata(EMetadata::Bytes));
   *((*(in.toObject()))["bytes"]->toBytes()) = QByteArray("test bytes");
   in.toObject()->insert("array",new EMetadata(EMetadata::Array));
   QList<EMetadata*>* array = (*(in.toObject()))["array"]->toArray();
   array->append(new EMetadata);
   array->append(new EMetadata(EMetadata::Bool));
   *(array->back()->toBool()) = true;
   array->append(new EMetadata(EMetadata::Double));
   *(array->back()->toDouble()) = 6.28;
   array->append(new EMetadata(EMetadata::String));
   *(array->back()->toString()) = QString("array test string");
   array->append(new EMetadata(EMetadata::Bytes));
   *(array->back()->toBytes()) = QByteArray("array test bytes");

   // write metadata to file and make sure it worked
   *stream << in;
   Q_ASSERT( (bool)*stream );

   // close stream and file
   delete stream;
   file.close();

   // reopen same file with new stream
   Q_ASSERT( file.open(QIODevice::ReadWrite) );
   stream = new EDataStream(&file);
   EMetadata out;

   // read in metadata and confirm it is the same
   *stream >> out;
   Q_ASSERT( (bool)*stream );
   Q_ASSERT( out.isObject() );
   Q_ASSERT( out.toObject()->size() == 6 );
   Q_ASSERT( (*(out.toObject()))["null"]->isNull() );
   Q_ASSERT( *((*(out.toObject()))["bool"]->toBool()) );
   Q_ASSERT( *((*(out.toObject()))["double"]->toDouble()) == 3.14 );
   Q_ASSERT( *((*(out.toObject()))["string"]->toString()) == QString("test string") );
   Q_ASSERT( *((*(out.toObject()))["bytes"]->toBytes()) == QByteArray("test bytes") );
   array = (*(out.toObject()))["array"]->toArray();
   Q_ASSERT( array->size() == 5 );
   Q_ASSERT( (*array)[0]->isNull() );
   Q_ASSERT( *((*array)[1]->toBool()) );
   Q_ASSERT( *((*array)[2]->toDouble()) == 6.28 );
   Q_ASSERT( *((*array)[3]->toString()) == QString("array test string") );
   Q_ASSERT( *((*array)[4]->toBytes()) == QByteArray("array test bytes") );

   // close stream and remove file
   delete stream;
   file.close();
   file.remove();
}
