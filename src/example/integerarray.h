#ifndef INTEGERARRAY_H
#define INTEGERARRAY_H
#include <core/core.h>
//



/*!
 */
class IntegerArray : public EAbstractData
{
   Q_OBJECT
public:
   virtual qint64 dataEnd() const override final;
   virtual void readData() override final;
   virtual void writeNewData() override final;
   virtual QAbstractTableModel* model() override final;
   virtual void prepare(bool preAllocate) override final;
   virtual void finish() override final;
   /*!
    */
   int _futureSize;
   /*!
    */
   QList<int> _numbers;
};



#endif
