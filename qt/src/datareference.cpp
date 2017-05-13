#include "datareference.h"






Ace::DataReference::DataReference(std::shared_ptr<Ace::DataObject> data
                                  , const QString &absolutePath):
   _data(data),
   _absolutePath(absolutePath)
{}






Ace::DataReference::~DataReference()
{
   _data.reset();
   emit released(_absolutePath);
}






Ace::DataObject& Ace::DataReference::operator*()
{
   return *_data;
}






Ace::DataObject* Ace::DataReference::operator->()
{
   return _data.get();
}
