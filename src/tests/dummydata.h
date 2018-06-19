#ifndef DUMMYDATA_H
#define DUMMYDATA_H
#include "eabstractdata.h"



class DummyData : public EAbstractData
{
public:
   virtual void readData() override final {}
   virtual quint64 getDataEnd() const override final { return 0; }
   virtual void newData() override final {}
   virtual void prepare(bool /*preAllocate*/) override final {}
   virtual void finish() override final {}
   virtual QAbstractTableModel* getModel() { return nullptr; }
   using EAbstractData::stream;
   bool getTouch() { return _touch; }
   void touch() { _touch = true; }
private:
   bool _touch {false};
};



#endif
