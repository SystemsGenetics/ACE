#ifndef TESTEXCEPTION_H
#define TESTEXCEPTION_H
#include <QtTest/QtTest>



class TestException : public QObject
{
   Q_OBJECT
private slots:
   void testSettersGetters();
};



#endif
