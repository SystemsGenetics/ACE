#include "datafactory.h"
#include "data.h"



using namespace std;






quint16 DataFactory::getCount()
{
   // Return final enumeration value which is size.
   return Total;
}






QString DataFactory::getName(quint16 type)
{
   switch (type)
   {
   case Integers:
      // This is Integers data object, return name.
      return QString("Integer Array");
   default:
      // This is an unknown type, return empty string.
      return QString();
   }
}






QString DataFactory::getFileExtension(quint16 type)
{
   switch (type)
   {
   case Integers:
      // This is Integers data object, return extension name.
      return QString("num");
   default:
      // This is an unknown type, return empty string.
      return QString();
   }
}






std::unique_ptr<EAbstractData> DataFactory::make(quint16 type)
{
   switch (type)
   {
   case Integers:
      // This is Integers data object, return new object.
      return unique_ptr<Data>(new Data());
   default:
      // This is unknown type, return null pointer.
      return nullptr;
   }
}
