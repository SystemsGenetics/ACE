#include "datafactory.h"
#include "dataframe.h"



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
   // Based off the data type given return its name. If the data type is not 
   // recognized then return an empty string. 
   switch (type)
   {
   case DataFrameType: return "Dataframe";
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
   // Based off the data type given return its file extension. If the data type is 
   // not recognized then return an empty string. 
   switch (type)
   {
   case DataFrameType: return "tab";
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
   // Create a new data object of the given type and return its pointer. If the data 
   // type given is not recognized then return a null pointer. 
   switch (type)
   {
   case DataFrameType: return unique_ptr<DataFrame>(new DataFrame());
   default: return nullptr;
   }
}
