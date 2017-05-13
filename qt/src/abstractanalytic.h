#ifndef EABSTRACTANALYTIC_H
#define EABSTRACTANALYTIC_H
#include <QtCore>

#include "utilities.h"



class AbstractData;



class EAbstractAnalytic : public QThread
{
   Q_OBJECT
public:
   enum class ArgumentType
   {
      Integer
      ,Float
      ,Double
      ,String
      ,FileIn
      ,FileOut
      ,DataObjectFileIn
      ,DataObjectFileOut
   };
   EAbstractAnalytic() = default;
   virtual ~EAbstractAnalytic() = default;
   virtual int getArgumentCount() = 0;
   virtual QStringList getCommandLineArguments() = 0;
   virtual ArgumentType getArgumentType(int argument) = 0;
   virtual QString getArgumentTitle(int argument) = 0;
   virtual QString getArgumentWhatsThis(int argument) = 0;
   virtual QStringList getFileArgumentFilters(int argument) = 0;
   virtual void setArgument(int argument, QVariant value) = 0;
   virtual void setArgument(int argument, QFile* file) = 0;
   virtual void setArgument(int argument, AbstractData* data) = 0;
   virtual void initialize() = 0;
   virtual int getBlockSize() = 0;
   virtual bool runBlock(int block) = 0;
   virtual void finish() = 0;
signals:
   void progressed(int perceptComplete);
   void finished();
private:
   void run() override final {}
};



#endif
