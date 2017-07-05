#ifndef TESTDATASTREAM_H
#define TESTDATASTREAM_H
#include <QtTest/QtTest>



class TestDataStream : public QObject
{
   Q_OBJECT
private slots:
   void testStreamOperators() {}
   void testBadString() {}
   void testBadBytes() {}
};



#endif
