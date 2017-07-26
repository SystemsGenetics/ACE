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
      ,DefaultValue
      ,Minimum
      ,Maximum
      ,Decimals
      ,CommandLineName
      ,ComboValues
      ,FileFilters
      ,DataType
   };
   EAbstractAnalytic() = default;
   virtual ~EAbstractAnalytic();
   virtual int getArgumentCount() = 0;
   virtual QString getCommandName() = 0;
   virtual ArgumentType getArgumentData(int argument) = 0;
   virtual QVariant getArgumentData(int argument, Role role) = 0;
   virtual void setArgument(int argument, QVariant value) = 0;
   virtual void setArgument(int argument, QFile* file) = 0;
   virtual void setArgument(int argument, EAbstractData* data) = 0;
   virtual bool initialize() = 0;
   virtual int getBlockSize() = 0;
   virtual bool runBlock(int block) = 0;
   virtual void finish() = 0;
   void run() override final;
   void addFileIn(int argument, const QString& path);
   void addFileOut(int argument, const QString& path);
   void addDataIn(int argument, const QString& path, quint16 type);
   void addDataOut(int argument, const QString& path, quint16 type);
   void setCommand(const QString& command) { _command = command; }
signals:
   void progressed(int perceptComplete);
   void finished();
protected:
   EAbstractData* getDataIn(const QString& path, quint16 type);
   EAbstractData* getDataOut(const QString& path, quint16 type);
private:
   static QMutex _mutex;
   QList<Ace::DataReference*> _dataIn;
   QList<Ace::DataReference*> _dataOut;
   QList<QFile*> _files;
   QString _command;
};



#endif
