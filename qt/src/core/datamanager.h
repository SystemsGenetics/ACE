#ifndef ACE_DATAMANAGER_H
#define ACE_DATAMANAGER_H
#include <QtCore>
#include <memory>

#include "dataobject.h"



namespace Ace { class DataReference; }



namespace Ace
{
   class DataManager : public QObject
   {
      Q_OBJECT
   public:
      enum Errors
      {
         NullReference = 0
      };
      static DataManager& getInstance() noexcept;
      std::unique_ptr<DataReference> open(const QString& path);
   public slots:
      void referenceReleased(const QString& absolutePath);
   private:
      DataManager() = default;
      static std::unique_ptr<DataManager> _instance;
      QMap<QString,std::shared_ptr<DataObject>> _dataObjects;
   };
}



#endif
