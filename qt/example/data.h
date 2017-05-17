#ifndef DATA_H
#define DATA_H
#include <AceCore.h>



class Data : public EAbstractData
{
public:
   void readData() override final;
   quint64 getDataEnd() const override final;
   void newData() override final;
   void prepare(bool preAllocate) override final;
   void finish() override final;
   QList<int> _numbers;
   int _futureSize {0};
};



#endif
