#include "exportdataframe_input.h"
#include "dataframe.h"
#include "datafactory.h"



//






/*!
 * Constructs a new input object with the given export dataframe as its parent.
 *
 * @param parent The parent analytic object for this new input object.
 */
ExportDataFrame::Input::Input(ExportDataFrame* parent):
   EAbstractAnalytic::Input(parent),
   _base(parent)
{}






/*!
 * Implements the interface that returns the total number of arguments this
 * analytic type contains.
 *
 * @return Total number of arguments for this analytic type.
 */
int ExportDataFrame::Input::size() const
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
EAbstractAnalytic::Input::Type ExportDataFrame::Input::type(int index) const
{
   // Based off the argument index given return its argument type. If the index is
   // out of range then return a boolean type.
   switch (index)
   {
   case InputData: return Type::DataIn;
   case OutputFile: return Type::FileOut;
   case NANToken: return Type::String;
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
QVariant ExportDataFrame::Input::data(int index, Role role) const
{
   // Based off the given argument index and role return the appropriate argument
   // data. If the index or the role are not recognized or supported then return
   // an empty variant.
   switch (index)
   {
   case InputData:
      switch (role)
      {
      case Role::CommandLineName: return "in";
      case Role::Title: return "Input dataframe:";
      case Role::WhatsThis: return "Dataframe that will have its data exported to raw text file.";
      case Role::DataType: return DataFactory::DataFrameType;
      default: return QVariant();
      }
   case OutputFile:
      switch (role)
      {
      case Role::CommandLineName: return "out";
      case Role::Title: return "Output file:";
      case Role::WhatsThis: return "Raw text file that will be overwritten with input data's integers.";
      case Role::FileFilters: return "Raw Text File (*.txt)";
      default: return QVariant();
      }
   case NANToken:
      switch (role)
      {
      case Role::CommandLineName: return QString("nan");
      case Role::Title: return tr("NAN Token:");
      case Role::WhatsThis: return tr("String token for NAN values.");
      case Role::Default: return "NA";
      default: return QVariant();
      }
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
void ExportDataFrame::Input::set(int index, const QVariant& value)
{
   // If the given index is for the NAN token argument then set the given string
   // value to this input's analytic.
   if ( index == NANToken )
   {
      _base->_nanToken = value.toString();
   }
}






/*!
 * Implements the interface that passes a pointer to the qt file device for a file
 * input/output argument with the given index.
 *
 * @param index Index of argument whose qt file device pointer is given.
 *
 * @param file Pointer to qt file device for the argument with the given index.
 */
void ExportDataFrame::Input::set(int index, QFile* file)
{
   // If the given index is for the input dataframe argument then set the data
   // object pointer to this input's analytic, casting it as an dataframe.
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
void ExportDataFrame::Input::set(int index, EAbstractData* data)
{
   // If the given index is for the output file argument then set the given qt file
   // pointer to this input's analytic.
   if ( index == InputData )
   {
      _base->_in = qobject_cast<DataFrame*>(data);
   }
}
