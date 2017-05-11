#ifndef EABSTRACTDATA_H
#define EABSTRACTDATA_H
#include <QtCore>
#include <memory>

#include "abstractdataiterator.h"
#include "utilities.h"



namespace Ace { class DataObject; }
class EDataStream;



class EAbstractData
{
public:
   EAbstractData() = default;
   virtual ~EAbstractData() = default;
   ACE_MAKE_NO_COPY_OR_MOVE(EAbstractData);
   virtual void readData() = 0;
   virtual quint64 getDataEnd() const = 0;
   virtual void prepare(bool preAllocate) = 0;
   virtual void finish() = 0;
   virtual std::unique_ptr<EAbstractDataIterator> begin() = 0;
   void initialize(Ace::DataObject* object);
protected:
   EDataStream& stream();
   bool seek(quint64 offset);
   bool allocate(quint64 size);
private:
   Ace::DataObject* _data {nullptr};
   EDataStream* _stream {nullptr};
};



#endif
