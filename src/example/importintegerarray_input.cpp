#include "importintegerarray_input.h"
#include "integerarray.h"
#include "datafactory.h"
//






/*!
 * Constructs a new input object with the given import integer array as its parent. 
 *
 * @param parent The parent analytic object for this new input object. 
 */
ImportIntegerArray::Input::Input(ImportIntegerArray* parent):
   EAbstractAnalytic::Input(parent),
   _base(parent)
{}






/*!
 * Implements the interface that returns the total number of arguments this 
 * analytic type contains. 
 *
 * @return Total number of arguments for this analytic type. 
 */
int ImportIntegerArray::Input::size() const
{
   return Total;
}






/*!
 * Implements the interface that returns the argument type for a given index. 
 *
 * @param index Index of argument whose type is returned. 
 *
 * @return Argument type for the given index. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Based off the argument index given return its argument type. If the index is 
 *    out of range then return a boolean type. 
 */
EAbstractAnalytic::Input::Type ImportIntegerArray::Input::type(int index) const
{
   // Step 1
   switch (index)
   {
   case InputFile: return Type::FileIn;
   case OutputData: return Type::DataOut;
   default: return Type::Boolean;
   }
}






/*!
 * Implements the interface that returns data for a given role on an argument with 
 * the given index. 
 *
 * @param index Index of argument whose data with the given role is returned. 
 *
 * @param role Role for the data that is returned. 
 *
 * @return Variant data for the given role and index. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Based off the argument index given call the method for getting argument data 
 *    for that index, returning what the method returns. 
 */
QVariant ImportIntegerArray::Input::data(int index, Role role) const
{
   // Step 1
   switch (index)
   {
   case InputFile: return inputFileData(role);
   case OutputData: return outputDataData(role);
   default: return QVariant();
   }
}






/*!
 * Implements the interface that sets an argument with the given index to the given 
 * value. This analytic has no basic arguments so this does nothing. 
 *
 * @param index Index of argument whose value is set to the given value. 
 *
 * @param value Variant value which is set to the argument with the given index. 
 */
void ImportIntegerArray::Input::set(int index, const QVariant& value)
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
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given index is for the input file argument then set the given qt file 
 *    pointer to this input's analytic. 
 */
void ImportIntegerArray::Input::set(int index, QFile* file)
{
   // Step 1
   if ( index == InputFile )
   {
      _base->_in = file;
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
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given index is for the output integer array argument then set the data 
 *    object pointer to this input's analytic, casting it as an integer array. 
 */
void ImportIntegerArray::Input::set(int index, EAbstractData* data)
{
   // Step 1
   if ( index == OutputData )
   {
      _base->_out = qobject_cast<IntegerArray*>(data);
   }
}






/*!
 * Returns argument data for the input file argument and the given role. 
 *
 * @param role Role for the data that is returned. 
 *
 * @return Argument data for the input file argument and given role. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Based off the role given return the appropriate argument data. If the role is 
 *    not recognized or supported then return an empty variant. 
 */
QVariant ImportIntegerArray::Input::inputFileData(Role role) const
{
   // Step 1
   switch (role)
   {
   case Role::CommandLineName: return "in";
   case Role::Title: return "Input Text File:";
   case Role::WhatsThis: return "Raw text file that contains a list of integers to import.";
   case Role::FileFilters: return "Raw Text File (*.txt)";
   default: return QVariant();
   }
}






/*!
 * Returns argument data for the output data argument and the given role. 
 *
 * @param role Role for the data that is returned. 
 *
 * @return Argument data for the output data argument and given role. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Based off the role given return the appropriate argument data. If the role is 
 *    not recognized or supported then return an empty variant. 
 */
QVariant ImportIntegerArray::Input::outputDataData(Role role) const
{
   // Step 1
   switch (role)
   {
   case Role::CommandLineName: return "out";
   case Role::Title: return "Output integer array:";
   case Role::WhatsThis: return "New integer array that will contain imported integers.";
   case Role::DataType: return DataFactory::IntegerArrayType;
   default: return QVariant();
   }
}
