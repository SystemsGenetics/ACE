#ifndef DATAREFERENCE_H
#define DATAREFERENCE_H
#include <QtCore>
#include <memory>



namespace Ace { class DataObject; }



namespace Ace
{
   class DataReference : public QObject
   {
      Q_OBJECT
   public:
      DataReference(std::shared_ptr<DataObject> data, const QString& absolutePath);
      ~DataReference();
      DataObject& operator*();
      DataObject* operator->();
   signals:
      void released(const QString& absolutePath);
   private:
      std::shared_ptr<DataObject> _data;
      QString _absolutePath;
   };
}



#endif
