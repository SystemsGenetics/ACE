#ifndef ABSTRACTANALYTICFACTORY_H
#define ABSTRACTANALYTICFACTORY_H
#include <QtCore>
#include <memory>

#include "abstractanalytic.h"
#include "utilities.h"



class EAbstractAnalyticFactory
{
public:
   EAbstractAnalyticFactory() = default;
   virtual ~EAbstractAnalyticFactory() = default;
   EMAKE_NO_COPY_OR_MOVE(EAbstractAnalyticFactory);
   static EAbstractAnalyticFactory& getInstance();
   static void setInstance(std::unique_ptr<EAbstractAnalyticFactory>&& factory);
   virtual int getCount() = 0;
   virtual QString getName(int type) = 0;
   virtual std::unique_ptr<EAbstractAnalytic> makeAnalytic(int type) = 0;
private:
   static std::unique_ptr<EAbstractAnalyticFactory> _instance;
};



#endif
