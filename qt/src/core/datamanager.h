#ifndef ACE_DATAMANAGER_H
#define ACE_DATAMANAGER_H
#include <QtCore>
#include <memory>

#include "dataobject.h"



namespace Ace { class DataReference; }



namespace Ace
{
   class DataManager
   {
   public:
      enum Errors
      {
         NullReference = 0
         ,CannotOpen
      };
      static DataManager& getInstance() noexcept;
      DataReference* open(const QString& path);
      void referenceReleased(const QString& absolutePath);
   private:
      DataManager() = default;
      static std::unique_ptr<DataManager> _instance;
      QMap<QString,std::shared_ptr<DataObject>> _dataObjects;
   };
}



#endif
