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






Ace::DataReference* Ace::DataManager::open(const QString& path)
{
   // touch the file to make sure it exists for absolute file pathfinding
   QFile touchFile(path);
   if ( !touchFile.open(QIODevice::ReadWrite) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Data Manager"));
      e.setDetails(QObject::tr("Cannot open file %1.").arg(path));
      throw e;
   }
   touchFile.close();

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

   // create new reference for data and return it
   unique_ptr<DataReference> reference(new DataReference(data,absolutePath));
   return new DataReference(data,absolutePath);
}






void Ace::DataManager::referenceReleased(const QString& absolutePath)
{
   // find data that is referenced
   auto i = _dataObjects.find(absolutePath);
   if ( i == _dataObjects.end() )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Internal Data Reference"));
      e.setDetails(QObject::tr("A data reference at path %1 was released when no data at that path"
                               " exists within the data manager.").arg(absolutePath));
      throw e;
   }

   // remove data from list if this was its last reference
   if ( i->use_count() == 1 )
   {
      _dataObjects.erase(i);
   }
}
