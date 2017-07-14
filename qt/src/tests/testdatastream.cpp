#include "testdatastream.h"
#include "datastream.h"






void TestDataStream::initTestCase()
{
   // open file that will be used
   _file = new QFile(QDir::tempPath() + "/ACE_test_io.raw");
   Q_ASSERT(_file->open(QIODevice::ReadWrite));
}






void TestDataStream::testStreamOperators()
{
   // initiailize all possible data types stream can handle
   qint8 vali8 = 21;
   quint8 valu8 = 22;
   qint16 vali16 = 23;
   quint16 valu16 = 24;
   qint32 vali32 = 25;
   quint32 valu32 = 26;
   qint64 vali64 = 27;
   quint64 valu64 = 28;
   float valfloat = 3.5;
   double valdouble = 3.75;
   QString valstring("test string");
   QByteArray valbytes("test bytes");

   // make new stream and write all data to file
   EDataStream stream(_file);
   stream << vali8 << valu8 << vali16 << valu16 << vali32 << valu32 << vali64 << valu64 << valfloat
          << valdouble << valstring << valbytes;

   // reset all data
   vali8 = valu8 = vali16 = valu16 = vali32 = valu32 = vali64 = valu64 = valfloat = valdouble = 0;
   valstring.clear();
   valbytes.clear();

   // read all data types just written back
   _file->seek(0);
   stream >> vali8 >> valu8 >> vali16 >> valu16 >> vali32 >> valu32 >> vali64 >> valu64 >> valfloat
          >> valdouble >> valstring >> valbytes;

   // make sure all data types are the same
   Q_ASSERT( vali8 == 21 );
   Q_ASSERT( valu8 == 22 );
   Q_ASSERT( vali16 == 23 );
   Q_ASSERT( valu16 == 24 );
   Q_ASSERT( vali32 == 25 );
   Q_ASSERT( valu32 == 26 );
   Q_ASSERT( vali64 == 27 );
   Q_ASSERT( valu64 == 28 );
   Q_ASSERT( valfloat == 3.5 );
   Q_ASSERT( valdouble == 3.75 );
   Q_ASSERT( valstring == QString("test string") );
   Q_ASSERT( QString(valbytes.data()) == QString("test bytes") );
}






void TestDataStream::testBadString()
{
   // initialize stream and string
   EDataStream stream(_file);
   QString value("not empty");

   // attempt to read string that doesn't exist
   _file->seek(0);
   stream >> value;

   // make sure it failed
   Q_ASSERT( value.isEmpty() );
   Q_ASSERT( !stream );
   Q_ASSERT( stream.getException().getType() == EDataStream::CorruptData );
}






void TestDataStream::testBadBytes()
{
   // initialize stream and bytes
   EDataStream stream(_file);
   QByteArray value("not empty");

   // attempt to read in bytes that doesn't exist
   _file->seek(0);
   stream >> value;

   // make sure it failed
   Q_ASSERT( value.isEmpty() );
   Q_ASSERT( !stream );
   Q_ASSERT( stream.getException().getType() == EDataStream::CorruptData );
}






void TestDataStream::cleanupTestCase()
{
   // cleanup temporary file
   _file->remove();
   delete _file;
}
