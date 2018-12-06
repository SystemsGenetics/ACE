#include "analyticfactory.h"
#include "mathtransform.h"
#include "importdataframe.h"
#include "exportdataframe.h"



using namespace std;
//






/*!
 * Implements the interface that returns the total number of analytic types this 
 * program implements. 
 *
 * @return Total number of analytic types this program implements. 
 */
quint16 AnalyticFactory::size() const
{
   return Total;
}






/*!
 * Implements the interface that returns the display name for the given analytic 
 * type. 
 *
 * @param type The analytic type whose display name is returned. 
 *
 * @return Display name for the given analytic type. 
 */
QString AnalyticFactory::name(quint16 type) const
{
   // Based off the analytic type given return its display name. If the analytic type 
   // is not recognized then return an empty string. 
   switch (type)
   {
   case ImportDataFrameType: return "Import Dataframe";
   case MathTransformType: return "Math Transform";
   case ExportDataFrameType: return "Export Dataframe";
   default: return QString();
   }
}






/*!
 * Implements the interface that returns the command line name for the given 
 * analytic type. 
 *
 * @param type The analytic type whose display name is returned. 
 *
 * @return Command line name for the given analytic type. 
 */
QString AnalyticFactory::commandName(quint16 type) const
{
   // Based off the analytic type given return its command line name. If the analytic 
   // type is not recognized then return an empty string. 
   switch (type)
   {
   case ImportDataFrameType: return "import";
   case MathTransformType: return "transform";
   case ExportDataFrameType: return "export";
   default: return QString();
   }
}






/*!
 * Implements the interface that makes and returns a new abstract analytic object 
 * of the given type. 
 *
 * @param type The data type of the abstract data object that is made and returned. 
 *
 * @return Pointer to the new abstract analytic object of the given type. 
 */
std::unique_ptr<EAbstractAnalytic> AnalyticFactory::make(quint16 type) const
{
   // Based off the analytic type given create a new object of that type and return 
   // its pointer. If the analytic type is not recognized then return a null pointer. 
   switch (type)
   {
   case ImportDataFrameType: return unique_ptr<ImportDataFrame>(new ImportDataFrame);
   case MathTransformType: return unique_ptr<MathTransform>(new MathTransform);
   case ExportDataFrameType: return unique_ptr<ExportDataFrame>(new ExportDataFrame);
   default: return nullptr;
   }
}
