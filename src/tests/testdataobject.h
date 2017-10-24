#ifndef TESTDATAOBJECT_H
#define TESTDATAOBJECT_H
#include <QtTest/QtTest>



namespace Ace { class DataObject; }



class TestDataObject : public QObject
{
   Q_OBJECT
private slots:
   void initTestCase();
   void testIsNew();
   void testClear();
   void testAllocate();
   void testSeek();
   void testBadClear();
   void testBadOpen();
   void cleanupTestCase();
private:
   QString _path;
   Ace::DataObject* _object;
};



#endif
