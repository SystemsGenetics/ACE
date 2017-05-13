#ifndef EABSTRACTANALYTICFACTORY_H
#define EABSTRACTANALYTICFACTORY_H
#include <QtCore>
#include <memory>

#include "utilities.h"



class EAbstractAnalytic;



class EAbstractAnalyticFactory
{
public:
   EAbstractAnalyticFactory() = default;
   virtual ~EAbstractAnalyticFactory() = default;
   ACE_DISBALE_COPY_AND_MOVE(EAbstractAnalyticFactory)
   static EAbstractAnalyticFactory& getInstance();
   static void setInstance(std::unique_ptr<EAbstractAnalyticFactory>&& factory);
   virtual quint16 getCount() = 0;
   virtual QString getName(quint16 type) = 0;
   virtual std::unique_ptr<EAbstractAnalytic> makeAnalytic(quint16 type) = 0;
private:
   static std::unique_ptr<EAbstractAnalyticFactory> _instance;
};



#endif
