#ifndef ABSTRACTANALYTIC_H
#define ABSTRACTANALYTIC_H
#include <QtCore>



class AbstractAnalytic
{
public:
   enum class ArgumentType
   {
      Integer
      ,Float
      ,Double
      ,String
      ,File
      ,DataObjectFileIn
      ,DataObjectFileOut
   };
   virtual ~AbstractAnalytic() = default;
   virtual int getArgumentCount() = 0;
   virtual QStringList getCommandLineArguments() = 0;
   virtual ArgumentType getArgumentType(int argument) = 0;
   virtual QString getArgumentTitle(int argument) = 0;
   virtual QString getArgumentWhatsThis(int argument) = 0;
   virtual QStringList getFileArgumentFilters(int argument) = 0;
   virtual void setArgument(int argument, QVariant value) = 0;
   virtual void setDataArgument(int argument/*, AbstractData* data*/) = 0;
   virtual void initialize() = 0;
   virtual int getBlockSize() = 0;
   virtual bool runBlock(int block) = 0;
   virtual void finish() = 0;
   bool hasInfoChanged();
   void markInfoRead();
   int getPercentComplete();
   QString getStatus();
protected:
   void setPercentComplete(int percent);
   void setStatus(QString status);
};



#endif
