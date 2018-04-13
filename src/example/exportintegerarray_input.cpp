#include "exportintegerarray_input.h"
#include "integerarray.h"
#include "datafactory.h"
//






/*!
 * Constructs a new input object with the given import integer array as its parent. 
 *
 * @param parent The parent analytic object for this new input object. 
 */
ExportIntegerArray::Input::Input(ExportIntegerArray* parent):
   EAbstractAnalytic::Input(parent),
   _base(parent)
{}






/*!
 * Implements the interface that returns the total number of arguments this 
 * analytic type contains. 
 *
 * @return Total number of arguments for this analytic type. 
 */
int ExportIntegerArray::Input::size() const
{
   return Total;
}






/*!
 * Implements the interface that returns the argument type for a given index. 
 *
 * @param index Index of argument whose type is returned. 
 *
 * @return Argument type for the given index. 
 */
EAbstractAnalytic::Input::Type ExportIntegerArray::Input::type(int index) const
{
   switch (index)
   {
   case InputData: return Type::DataIn;
   case OutputFile: return Type::FileOut;
   default: return Type::Boolean;
   }
}






/*!
 * This interface returns data for a given role on an argument with the given 
 * index. 
 *
 * @param index Index of argument whose data with the given role is returned. 
 *
 * @param role Role for the data that is returned. 
 *
 * @return Variant data for the given role and index. 
 */
QVariant ExportIntegerArray::Input::data(int index, Role role) const
{
   switch (index)
   {
   case InputData: return inputDataData(role);
   case OutputFile: return outputFileData(role);
   default: return QVariant();
   }
}






/*!
 * Implements the interface that sets an argument with the given index to the given 
 * value. 
 *
 * @param index Index of argument whose value is set to the given value. 
 *
 * @param value Variant value which is set to the argument with the given index. 
 */
void ExportIntegerArray::Input::set(int index, const QVariant& value)
{
   Q_UNUSED(index)
   Q_UNUSED(value)
}






/*!
 * Implements the interface that passes a pointer to the qt file device for a file 
 * input/output argument with the given index. 
 *
 * @param index Index of argument whose qt file device pointer is given. 
 *
 * @param file Pointer to qt file device for the argument with the given index. 
 */
void ExportIntegerArray::Input::set(int index, QFile* file)
{
   if ( index == OutputFile )
   {
      _base->_out = file;
   }
}






/*!
 * Implement the interface that passes a pointer to the abstract data object for a 
 * data input/output argument with the given index. 
 *
 * @param index Index of argument whose abstract data object pointer is given. 
 *
 * @param data Pointer to abstract data object for the argument with the given 
 *             index. 
 */
void ExportIntegerArray::Input::set(int index, EAbstractData* data)
{
   if ( index == InputData )
   {
      _base->_in = qobject_cast<IntegerArray*>(data);
   }
}






/*!
 *
 * @param role  
 */
QVariant ExportIntegerArray::Input::inputDataData(Role role) const
{
   switch (role)
   {
   case Role::CommandLineName: return "in";
   case Role::Title: return "Input integer array:";
   case Role::WhatsThis: return "Integer array that will have its integers exported to raw text file.";
   case Role::DataType: return DataFactory::IntegerArrayType;
   default: return QVariant();
   }
}






/*!
 *
 * @param role  
 */
QVariant ExportIntegerArray::Input::outputFileData(Role role) const
{
   switch (role)
   {
   case Role::CommandLineName: return "out";
   case Role::Title: return "Output file:";
   case Role::WhatsThis: return "Raw text file that will be overwritten with input data's integers.";
   case Role::FileFilters: return "Raw Text File (*.txt)";
   default: return QVariant();
   }
}
