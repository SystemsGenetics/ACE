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
      static DataManager& getInstance();
      std::unique_ptr<DataReference> open(const QString& path);
      DataObject::Status getLastOpenStatus() const;
   public slots:
      void referenceReleased(const QString& absolutePath);
   private:
      DataManager() = default;
      static std::unique_ptr<DataManager> _instance;
      QMap<QString,std::shared_ptr<DataObject>> _dataObjects;
      DataObject::Status _lastOpenStatus {DataObject::Ok};
   };
}



#endif
