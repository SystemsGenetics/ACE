#ifndef ABSTRACTDATA_H
#define ABSTRACTDATA_H
#include <QtCore>
#include <memory>

#include "abstractdataiterator.h"



class DataObject;
class DataStream;



class AbstractData
{
public:
   virtual ~AbstractData() = default;
   virtual bool readData() = 0;
   virtual quint64 getDataEnd() const = 0;
   virtual bool prepare() = 0;
   virtual bool initialize(bool preAllocate) = 0;
   virtual void finish() = 0;
   virtual std::unique_ptr<AbstractDataIterator> begin() = 0;
   template<class T> static std::unique_ptr<T> makeInstance(DataObject* object);
protected:
   DataStream& stream();
   bool seek(quint64 offset);
   bool allocate(quint64 size);
private:
   AbstractData() = default;
   DataObject* _data {nullptr};
};


template<class T>
std::unique_ptr<T> AbstractData::makeInstance(DataObject *object)
{
}



#endif
