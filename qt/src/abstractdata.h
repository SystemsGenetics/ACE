#ifndef ABSTRACTDATA_H
#define ABSTRACTDATA_H
#include <QtCore>
#include <memory>

#include "abstractdataiterator.h"
#include "utilities.h"



class DataObject;
class DataStream;



class AbstractData
{
public:
   AbstractData() = default;
   virtual ~AbstractData() = default;
   MAKE_NO_COPY_OR_MOVE(AbstractData);
   virtual void readData() = 0;
   virtual quint64 getDataEnd() const = 0;
   virtual void prepare(bool preAllocate) = 0;
   virtual void finish() = 0;
   virtual std::unique_ptr<AbstractDataIterator> begin() = 0;
   void initialize(DataObject* object);
protected:
   DataStream& stream();
   bool seek(quint64 offset);
   bool allocate(quint64 size);
private:
   DataObject* _data {nullptr};
   DataStream* _stream {nullptr};
};



#endif
