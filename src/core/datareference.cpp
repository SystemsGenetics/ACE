#include "datareference.h"
#include "datamanager.h"






Ace::DataReference::DataReference(std::shared_ptr<Ace::DataObject> data
                                  , const QString &absolutePath):
   _data(data),
   _absolutePath(absolutePath)
{}






Ace::DataReference::~DataReference()
{
   // shared pointer must be reset first before released signal emitted
   _data.reset();
   DataManager::getInstance().referenceReleased(_absolutePath);
}






Ace::DataObject& Ace::DataReference::operator*()
{
   return *_data;
}






Ace::DataObject* Ace::DataReference::operator->()
{
   return _data.get();
}
