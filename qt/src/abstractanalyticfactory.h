#ifndef ABSTRACTANALYTICFACTORY_H
#define ABSTRACTANALYTICFACTORY_H
#include <QtCore>
#include <memory>

#include "abstractanalytic.h"



class AbstractAnalyticFactory
{
public:
   virtual ~AbstractAnalyticFactory() = default;
   static AbstractAnalyticFactory& getInstance();
   static void setInstance(std::unique_ptr<AbstractAnalyticFactory>&& factory);
   virtual int getCount() = 0;
   virtual QString getName(int type) = 0;
   virtual std::unique_ptr<AbstractAnalytic> makeAnalytic(int type) = 0;
private:
   static AbstractAnalyticFactory* _instance;
};



#endif
