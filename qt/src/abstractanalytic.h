#ifndef ABSTRACTANALYTIC_H
#define ABSTRACTANALYTIC_H
#include <QtCore>



class AbstractData;



class AbstractAnalytic
{
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
   virtual ~AbstractAnalytic() = default;
   AbstractAnalytic(const AbstractAnalytic&) = delete;
   AbstractAnalytic& operator=(const AbstractAnalytic&) = delete;
   AbstractAnalytic(AbstractAnalytic&&) = delete;
   AbstractAnalytic& operator=(AbstractAnalytic&&) = delete;
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
   int getPercentComplete();
   QString getStatus();
protected:
   void setPercentComplete(int percent);
   void setStatus(QString status);
private:
   int _percentComplete;
   QString _status;
};



#endif
