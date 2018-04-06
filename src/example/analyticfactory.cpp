#include "analyticfactory.h"
#include "mathtransform.h"
#include "importintegerarray.h"
#include "exportintegerarray.h"



using namespace std;
//






/*!
 * Implements the interface that returns the total number of analytic types this 
 * program. 
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
   switch (type)
   {
   case ImportIntegerArrayType: return "Import Integer Array";
   case MathTransformType: return "Math Transform";
   case ExportIntegerArrayType: return "Export Integer Array";
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
   switch (type)
   {
   case ImportIntegerArrayType: return "import";
   case MathTransformType: return "transform";
   case ExportIntegerArrayType: return "export";
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
   switch (type)
   {
   case ImportIntegerArrayType: return unique_ptr<ImportIntegerArray>(new ImportIntegerArray);
   case MathTransformType: return unique_ptr<MathTransform>(new MathTransform);
   case ExportIntegerArrayType: return unique_ptr<ExportIntegerArray>(new ExportIntegerArray);
   default: return nullptr;
   }
}
