#include "ace_datamanager.h"



using namespace Ace;
//



/*!
 * Pointer to global instance the data manager object. 
 */
Ace::DataManager* DataManager::_instance {nullptr};






/*!
 * Returns a reference to the global instance of this manager. If no instance has 
 * been created yet this created a new global instance. 
 *
 * @return Reference to global instance of this manager. 
 */
Ace::DataManager& DataManager::instance()
{
   if ( !_instance )
   {
      _instance = new DataManager;
   }
   return *_instance;
}






/*!
 * Called when a new data object is being created with the given canonical path, 
 * overwriting any data object that shares the same path on the file system. 
 *
 * @param canonicalPath Canonical path of new data object overwriting any other 
 *                      data object that shares the same path. 
 *
 * @param object Pointer to the new data object overwriting the given file path. 
 */
void DataManager::newDataOpened(const QString& canonicalPath, Ace::DataObject* object)
{
   emit dataOverwritten(canonicalPath,object);
}
