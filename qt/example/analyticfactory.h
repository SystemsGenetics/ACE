#ifndef ANALYTICFACTORY_H
#define ANALYTICFACTORY_H
#include <AceCore.h>



class AnalyticFactory : public EAbstractAnalyticFactory
{
public:
   quint16 getCount() override final;
   QString getName(quint16 type) override final;
   std::unique_ptr<EAbstractAnalytic> makeAnalytic(quint16 type);
};



#endif
