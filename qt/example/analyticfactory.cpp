#include "analyticfactory.h"
#include "mathtransform.h"



using namespace std;






quint16 AnalyticFactory::getCount()
{
   // Get total of possible types using enumeration
   return Total;
}






QString AnalyticFactory::getName(quint16 type)
{
   switch (type)
   {
   case MathTransformType:
      // This is math transform type, return name
      return QString("Math Transform");
   default:
      // This is unknown type, return empty string
      return QString();
   }
}






unique_ptr<EAbstractAnalytic> AnalyticFactory::make(quint16 type)
{
   switch (type)
   {
   case MathTransformType:
      // This is math transform type, return new object
      return unique_ptr<MathTransform>(new MathTransform());
   default:
      // This is unkonwn type, return null pointer
      return nullptr;
   }
}
