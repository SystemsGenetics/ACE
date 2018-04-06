#include "datafactory.h"
#include "integerarray.h"



using namespace std;
//






/*!
 * Implements the interface that returns the total number of data types this 
 * program implements. 
 *
 * @return Total number of data types this program implements. 
 */
quint16 DataFactory::size() const
{
   return Total;
}






/*!
 * Implements the interface that returns the display name for the given data type. 
 *
 * @param type The data type whose display name is returned. 
 *
 * @return Display name for the given data type. 
 */
QString DataFactory::name(quint16 type) const
{
   switch (type)
   {
   case IntegerArrayType: return "Integer Array";
   default: return QString();
   }
}






/*!
 * Implements the interface that returns the file extension for the given data type 
 * as a string. 
 *
 * @param type The data type whose file extension is returned. 
 *
 * @return File extension for the given data type. 
 */
QString DataFactory::fileExtension(quint16 type) const
{
   switch (type)
   {
   case IntegerArrayType: return "num";
   default: return QString();
   }
}






/*!
 * Implements the interface that makes and returns a new abstract data object of 
 * the given type. 
 *
 * @param type The data type of the abstract data object that is made and returned. 
 *
 * @return Pointer to the new abstract data object of the given type. 
 */
std::unique_ptr<EAbstractData> DataFactory::make(quint16 type) const
{
   switch (type)
   {
   case IntegerArrayType: return unique_ptr<IntegerArray>(new IntegerArray());
   default: return nullptr;
   }
}
