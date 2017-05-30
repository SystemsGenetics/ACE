#ifndef ANALYTICFACTORY_H
#define ANALYTICFACTORY_H
#include <core/AceCore.h>



// Analytic factory
class AnalyticFactory : public EAbstractAnalyticFactory
{
public:
   // Make enumeration of all possible analytic types
   enum Types
   {
      MathTransformType = 0,
      Total
   };
   quint16 getCount() override final;
   QString getName(quint16 type) override final;
   std::unique_ptr<EAbstractAnalytic> make(quint16 type);
};



#endif
