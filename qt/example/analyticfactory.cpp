#include "analyticfactory.h"
#include "analytic.h"



using namespace std;






quint16 AnalyticFactory::getCount()
{
   return Total;
}






QString AnalyticFactory::getName(quint16 type)
{
   switch (type)
   {
   case MathTransform:
      return QString("Math Transform");
   default:
      return QString();
   }
}






unique_ptr<EAbstractAnalytic> AnalyticFactory::make(quint16 type)
{
   switch (type)
   {
   case MathTransform:
      return unique_ptr<Analytic>(new Analytic());
   default:
      return nullptr;
   }
}
