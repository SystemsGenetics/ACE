#ifndef TESTOPENCLDEVICE_H
#define TESTOPENCLDEVICE_H
#include <QtTest>



class TestOpenCLDevice : public QObject
{
   Q_OBJECT
private slots:
   void testGetters();
   void testMakers();
};



#endif
