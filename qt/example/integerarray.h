#ifndef DATA_H
#define DATA_H
#include <AceCore.h>



// Data type that implements the Integer type from factory
//
// This data object stores a one dimensional array of integer values
class IntegerArray : public EAbstractData
{
public:
   void readData() override final;
   quint64 getDataEnd() const override final;
   void newData() override final;
   void prepare(bool preAllocate) override final;
   void finish() override final;
   // This is a contrived example, data should be private and iterator classes defined to access
   // it
   QList<int> _numbers;
   int _futureSize {0};
};



#endif
