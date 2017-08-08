#include "analyticfactory.h"
#include "mathtransform.h"
#include "importintegerarray.h"
#include "exportintegerarray.h"



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
   case ImportIntegerArrayType:
      // This is import integer array type, return name
      return QString("Import Integer Array");
   case MathTransformType:
      // This is math transform type, return name
      return QString("Math Transform");
   case ExportIntegerArrayType:
      // This is export integer array type, return name
      return QString("Export Integer Array");
   default:
      // This is unknown type, return empty string
      return QString();
   }
}






unique_ptr<EAbstractAnalytic> AnalyticFactory::make(quint16 type)
{
   switch (type)
   {
   case ImportIntegerArrayType:
      // This is import integer array type, return new object
      return unique_ptr<ImportIntegerArray>(new ImportIntegerArray());
   case MathTransformType:
      // This is math transform type, return new object
      return unique_ptr<MathTransform>(new MathTransform());
   case ExportIntegerArrayType:
      // This is import integer array type, return new object
      return unique_ptr<ExportIntegerArray>(new ExportIntegerArray());
   default:
      // This is unkonwn type, return null pointer
      return nullptr;
   }
}
