#include "datafactory.h"
#include "data.h"



using namespace std;






quint16 DataFactory::getCount()
{
   return Total;
}






QString DataFactory::getName(quint16 type)
{
   switch (type)
   {
   case Integers:
      return QString("Integer Array");
   default:
      return QString();
   }
}






QString DataFactory::getFileExtension(quint16 type)
{
   switch (type)
   {
   case Integers:
      return QString("num");
   default:
      return QString();
   }
}






std::unique_ptr<EAbstractData> DataFactory::make(quint16 type)
{
   switch (type)
   {
   case Integers:
      return unique_ptr<Data>(new Data());
   default:
      return nullptr;
   }
}
