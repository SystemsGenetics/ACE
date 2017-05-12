#include "analyticfactory.h"



using namespace std;






quint16 AnalyticFactory::getCount()
{
   return 3;
}






QString AnalyticFactory::getName(quint16 type)
{
   switch (type)
   {
   case 0:
      return QString("Test 1");
   case 1:
      return QString("Test 2");
   case 2:
      return QString("Test 3");
   default:
      return QString();
   }
}






unique_ptr<EAbstractAnalytic> AnalyticFactory::makeAnalytic(quint16 /*type*/)
{
   return nullptr;
}
