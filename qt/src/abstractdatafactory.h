#ifndef EABSTRACTDATAFACTORY_H
#define EABSTRACTDATAFACTORY_H
#include <memory>

#include "utilities.h"



class EAbstractData;



class EAbstractDataFactory
{
public:
   EAbstractDataFactory() = default;
   virtual ~EAbstractDataFactory() = default;
   ACE_DISBALE_COPY_AND_MOVE(EAbstractDataFactory)
   static EAbstractDataFactory& getInstance();
   static void setInstance(std::unique_ptr<EAbstractDataFactory>&& factory);
   virtual quint16 getCount() = 0;
   virtual QString getName(quint16 type) = 0;
   virtual QString getFileExtension(quint16 type) = 0;
   virtual std::unique_ptr<EAbstractData> make(quint16 type) = 0;
private:
   static std::unique_ptr<EAbstractDataFactory> _instance;
};



#endif
