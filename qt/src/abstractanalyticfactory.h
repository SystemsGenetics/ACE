#ifndef ABSTRACTANALYTICFACTORY_H
#define ABSTRACTANALYTICFACTORY_H
#include <QtCore>
#include <memory>

#include "abstractanalytic.h"
#include "utilities.h"



class AbstractAnalyticFactory
{
public:
   AbstractAnalyticFactory() = default;
   virtual ~AbstractAnalyticFactory() = default;
   MAKE_NO_COPY_OR_MOVE(AbstractAnalyticFactory);
   static AbstractAnalyticFactory& getInstance();
   static void setInstance(std::unique_ptr<AbstractAnalyticFactory>&& factory);
   virtual int getCount() = 0;
   virtual QString getName(int type) = 0;
   virtual std::unique_ptr<AbstractAnalytic> makeAnalytic(int type) = 0;
private:
   static std::unique_ptr<AbstractAnalyticFactory> _instance;
};



#endif
