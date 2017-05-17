#ifndef ANALYTICFACTORY_H
#define ANALYTICFACTORY_H
#include <AceCore.h>



class AnalyticFactory : public EAbstractAnalyticFactory
{
public:
   enum Types
   {
      MathTransform = 0,
      Total
   };
   quint16 getCount() override final;
   QString getName(quint16 type) override final;
   std::unique_ptr<EAbstractAnalytic> make(quint16 type);
};



#endif
