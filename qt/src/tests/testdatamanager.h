#ifndef TESTDATAMANAGER_H
#define TESTDATAMANAGER_H
#include <QtTest>



class TestDataManager : public QObject
{
   Q_OBJECT
private slots:
   void initTestCase();
   void testOpen();
   void testDestruction();
   void cleanupTestCase();
private:
   QString _path;
};



#endif
