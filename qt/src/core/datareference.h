#ifndef DATAREFERENCE_H
#define DATAREFERENCE_H
#include <QtCore>
#include <memory>



namespace Ace { class DataObject; }



namespace Ace
{
   class DataReference
   {
   public:
      DataReference(std::shared_ptr<DataObject> data, const QString& absolutePath) noexcept;
      ~DataReference() noexcept;
      DataObject& operator*() noexcept;
      DataObject* operator->() noexcept;
   private:
      std::shared_ptr<DataObject> _data;
      QString _absolutePath;
   };
}



#endif
