#include "datamanager.h"
#include "datareference.h"
#include "exception.h"



using namespace std;
unique_ptr<Ace::DataManager> Ace::DataManager::_instance {nullptr};






Ace::DataManager& Ace::DataManager::getInstance() noexcept
{
   if ( !_instance )
   {
      _instance.reset(new DataManager());
   }
   return *_instance;
}






unique_ptr<Ace::DataReference> Ace::DataManager::open(const QString& path)
{
   // get absolute file path and prepare pointer
   QFileInfo fileInfo(path);
   QString absolutePath = fileInfo.canonicalFilePath();
   shared_ptr<DataObject> data;

   // see if file already exists within manager list
   auto i = _dataObjects.find(absolutePath);
   if ( i != _dataObjects.end() )
   {
      // assign data pointer
      data = *i;
   }
   else
   {
      // it does not exist, make new data object with given path and insert into list
      data.reset(new DataObject(path));
      _dataObjects.insert(absolutePath,data);
   }

   // create new reference for data, connect released signal, and return it
   unique_ptr<DataReference> reference(new DataReference(data,absolutePath));
   connect(reference.get(),SIGNAL(released(const QString&)),this
           ,SLOT(referenceReleased(const QString&)));
   return unique_ptr<DataReference>(new DataReference(data,absolutePath));
}






void Ace::DataManager::referenceReleased(const QString& absolutePath)
{
   // find data that is referenced
   auto i = _dataObjects.find(absolutePath);
   if ( i == _dataObjects.end() )
   {
      E_MAKE_EXCEPTION(e);
      e.setLevel(EException::Critical);
      e.setType(NullReference);
      e.setTitle(tr("Internal Data Reference"));
      e.setDetails(tr("A data reference at path %1 was released when no data at that path exists"
                      " within the data manager.").arg(absolutePath));
      throw e;
   }

   // remove data from list if this was its last reference
   if ( i->use_count() == 1 )
   {
      _dataObjects.erase(i);
   }
}
