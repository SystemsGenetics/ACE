#ifndef ABSTRACTDATAFACTORY_H
#define ABSTRACTDATAFACTORY_H
#include <memory>

#include "utilities.h"
#include "abstractdata.h"



class AbstractDataFactory
{
public:
   AbstractDataFactory() = default;
   virtual ~AbstractDataFactory() = default;
   MAKE_NO_COPY_OR_MOVE(AbstractDataFactory);
   static AbstractDataFactory& getInstance();
   static void setInstance(std::unique_ptr<AbstractDataFactory>&& factory);
   virtual int getCount() = 0;
   virtual QString getName(int type) = 0;
   virtual QString getFileExtension(int type) = 0;
   virtual std::unique_ptr<AbstractData> makeData(int type) = 0;
private:
   static std::unique_ptr<AbstractDataFactory> _instance;
};



#endif
