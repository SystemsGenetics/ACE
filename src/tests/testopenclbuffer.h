#ifndef TESTOPENCLBUFFER_H
#define TESTOPENCLBUFFER_H
#include <QtTest>



class TestOpenCLBuffer : public QObject
{
   Q_OBJECT
private slots:
   void testReadWrite();
};



#endif
