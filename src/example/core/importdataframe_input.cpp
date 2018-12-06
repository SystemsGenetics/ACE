#include "importdataframe_input.h"
#include "dataframe.h"
#include "datafactory.h"



//






/*!
 * Constructs a new input object with the given import dataframe as its parent.
 *
 * @param parent The parent analytic object for this new input object.
 */
ImportDataFrame::Input::Input(ImportDataFrame* parent):
   EAbstractAnalytic::Input(parent),
   _base(parent)
{}






/*!
 * Implements the interface that returns the total number of arguments this
 * analytic type contains.
 *
 * @return Total number of arguments for this analytic type.
 */
int ImportDataFrame::Input::size() const
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
EAbstractAnalytic::Input::Type ImportDataFrame::Input::type(int index) const
{
   // Based off the argument index given return its argument type. If the index is
   // out of range then return a boolean type.
   switch (index)
   {
   case InputFile: return Type::FileIn;
   case OutputData: return Type::DataOut;
   case NANToken: return Type::String;
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
 */
QVariant ImportDataFrame::Input::data(int index, Role role) const
{
   // Based off the given argument index and role return the appropriate argument
   // data. If the index or the role are not recognized or supported then return
   // an empty variant.
   switch (index)
   {
   case InputFile:
      switch (role)
      {
      case Role::CommandLineName: return "in";
      case Role::Title: return "Input Text File:";
      case Role::WhatsThis: return "Raw text file that contains a table of data to import.";
      case Role::FileFilters: return "Raw Text File (*.txt)";
      default: return QVariant();
      }
   case OutputData:
      switch (role)
      {
      case Role::CommandLineName: return "out";
      case Role::Title: return "Output dataframe:";
      case Role::WhatsThis: return "New dataframe that will contain imported table.";
      case Role::DataType: return DataFactory::DataFrameType;
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
void ImportDataFrame::Input::set(int index, const QVariant& value)
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
void ImportDataFrame::Input::set(int index, QFile* file)
{
   // If the given index is for the input file argument then set the given qt file
   // pointer to this input's analytic.
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
 */
void ImportDataFrame::Input::set(int index, EAbstractData* data)
{
   // If the given index is for the output dataframe argument then set the data
   // object pointer to this input's analytic, casting it as an dataframe.
   if ( index == OutputData )
   {
      _base->_out = qobject_cast<DataFrame*>(data);
   }
}
