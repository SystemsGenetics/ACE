#ifndef EABSTRACTDATAFACTORY_H
#define EABSTRACTDATAFACTORY_H
#include <memory>

#include "utilities.h"
#include "abstractdata.h"



class EAbstractDataFactory
{
public:
   EAbstractDataFactory() = default;
   virtual ~EAbstractDataFactory() = default;
   ACE_MAKE_NO_COPY_OR_MOVE(EAbstractDataFactory);
   static EAbstractDataFactory& getInstance();
   static void setInstance(std::unique_ptr<EAbstractDataFactory>&& factory);
   virtual int getCount() = 0;
   virtual QString getName(int type) = 0;
   virtual QString getFileExtension(int type) = 0;
   virtual std::unique_ptr<EAbstractData> makeData(int type) = 0;
private:
   static std::unique_ptr<EAbstractDataFactory> _instance;
};



#endif
