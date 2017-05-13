#include "datamanager.h"
#include "datareference.h"
#include "exception.h"



using namespace std;
unique_ptr<Ace::DataManager> Ace::DataManager::_instance {nullptr};






Ace::DataManager& Ace::DataManager::getInstance()
{
   if ( !_instance )
   {
      _instance.reset(new DataManager());
   }
   return *_instance;
}






unique_ptr<Ace::DataReference> Ace::DataManager::open(const QString& path)
{
   QFileInfo fileInfo(path);
   QString absolutePath = fileInfo.canonicalPath() + fileInfo.fileName();
   auto i = _dataObjects.find(absolutePath);
   if ( i != _dataObjects.end() )
   {
      return unique_ptr<DataReference>(new DataReference(*i,absolutePath));
   }
   shared_ptr<DataObject> data(new DataObject(path));
   _lastOpenStatus = data->getStatus();
   if ( !*data )
   {
      return nullptr;
   }
   _dataObjects.insert(absolutePath,data);
   return unique_ptr<DataReference>(new DataReference(data,absolutePath));
}






Ace::DataObject::Status Ace::DataManager::getLastOpenStatus() const
{
   return _lastOpenStatus;
}






void Ace::DataManager::referenceReleased(const QString& absolutePath)
{
   auto i = _dataObjects.find(absolutePath);
   if ( i == _dataObjects.end() )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Internal Data Reference Error"));
      e.out() << tr("A data reference at path %1 was released when no data at that path exists"
                    " within the data manager.").arg(absolutePath);
      throw e;
   }
   if ( i->use_count() == 1 )
   {
      _dataObjects.erase(i);
   }
}
