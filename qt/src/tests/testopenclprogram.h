#ifndef TESTOPENCLPROGRAM_H
#define TESTOPENCLPROGRAM_H
#include <QtTest>



class EOpenCLProgram;



class TestOpenCLProgram : public QObject
{
   Q_OBJECT
private slots:
   void testFromSource();
   void testFromFile();
   void testBadCompile();
private:
   EOpenCLProgram* _program;
};



#endif
