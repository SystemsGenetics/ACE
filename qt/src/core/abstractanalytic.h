#ifndef EABSTRACTANALYTIC_H
#define EABSTRACTANALYTIC_H
#include <QtCore>

#include "utilities.h"



namespace Ace { class DataReference; }
class EAbstractData;



/// Interface class that defines an analytic type.
class EAbstractAnalytic : public QThread
{
   Q_OBJECT
public:
   /// All possible argument types.
   enum class ArgumentType
   {
      Bool
      ,Integer
      ,Float
      ,Double
      ,String
      ,Combo
      ,FileIn
      ,FileOut
      ,DataIn
      ,DataOut
   };
   enum class Role
   {
      Type
      ,Title
      ,WhatsThis
      ,Required
      ,DefaultValue
      ,Minimum
      ,Maximum
      ,CommandLineName
      ,ComboValues
      ,FileFilters
      ,DataType
   };
   EAbstractAnalytic() = default;
   virtual ~EAbstractAnalytic() { qDeleteAll(_extraDatas); }
   virtual int getArgumentCount() = 0;
   virtual QVariant getArgumentData(Role role) = 0;
   virtual void setArgument(int argument, QVariant value) = 0;
   virtual void setArgument(int argument, QFile* file) = 0;
   virtual void setArgument(int argument, EAbstractData* data) = 0;
   virtual bool initialize() = 0;
   virtual int getBlockSize() = 0;
   virtual bool runBlock(int block) = 0;
   virtual void finish() = 0;
   void run() override final;
   QList<Ace::DataReference*> getExtraDatas() { return _extraDatas; }
signals:
   void progressed(int perceptComplete);
   void finished();
protected:
   EAbstractData* getExtraData(const QString& path);
private:
   static QMutex _mutex;
   QList<Ace::DataReference*> _extraDatas;
};



#endif
