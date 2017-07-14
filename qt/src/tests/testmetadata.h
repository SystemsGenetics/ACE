#ifndef TESTMETADATA_H
#define TESTMETADATA_H
#include <QtTest>



class TestMetadata : public QObject
{
   Q_OBJECT
private slots:
   void testBool();
   void testDouble();
   void testString();
   void testBytes();
   void testArray();
   void testObject();
   void testNull();
   void testFileInputOutput();
};



#endif
