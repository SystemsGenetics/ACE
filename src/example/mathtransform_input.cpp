#include "mathtransform_input.h"
#include <limits>
#include "datafactory.h"
#include "integerarray.h"
//



/*!
 */
const QStringList MathTransform::Input::_operationNames
{
   "addition"
   ,"subtractino"
   ,"multiplication"
   ,"division"
};






/*!
 * Constructs a new input object with the given math transform as its parent. 
 *
 * @param parent The parent math transform object for this new input object. 
 */
MathTransform::Input::Input(MathTransform* parent):
   EAbstractAnalytic::Input(parent),
   _base(parent)
{}






/*!
 * Implements the interface that returns the total number of arguments this 
 * analytic type contains. 
 *
 * @return Total number of arguments for this analytic type. 
 */
int MathTransform::Input::size() const
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
EAbstractAnalytic::Input::Type MathTransform::Input::type(int index) const
{
   switch (index)
   {
   case InputData: return Type::DataIn;
   case OutputData: return Type::DataOut;
   case OperationType: return Type::Selection;
   case Amount: return Type::Integer;
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
QVariant MathTransform::Input::data(int index, Role role) const
{
   switch (index)
   {
   case InputData: return inputDataData(role);
   case OutputData: return outputDataData(role);
   case OperationType: return operationTypeData(role);
   case Amount: return amountData(role);
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
void MathTransform::Input::set(int index, const QVariant& value)
{
   switch (index)
   {
   case OperationType:
      _base->_type = static_cast<Operation>(_operationNames.indexOf(value.toString()));
      break;
   case Amount:
      _base->_amount = value.toInt();
      break;
   }
}






/*!
 * Implements the interface that passes a pointer to the qt file device for a file 
 * input/output argument with the given index. This implementation does nothing 
 * because this analytic has no file arguments. 
 *
 * @param index Index of argument whose qt file device pointer is given. 
 *
 * @param file Pointer to qt file device for the argument with the given index. 
 */
void MathTransform::Input::set(int index, QFile* file)
{
   Q_UNUSED(index)
   Q_UNUSED(file)
}






/*!
 * Implements the interface that passes a pointer to the abstract data object for a 
 * data input/output argument with the given index. 
 *
 * @param index Index of argument whose abstract data object pointer is given. 
 *
 * @param data Pointer to abstract data object for the argument with the given 
 *             index. 
 */
void MathTransform::Input::set(int index, EAbstractData* data)
{
   switch (index)
   {
   case InputData:
      _base->_in = qobject_cast<IntegerArray*>(data);
      break;
   case OutputData:
      _base->_out = qobject_cast<IntegerArray*>(data);
      break;
   }
}






/*!
 *
 * @param role  
 */
QVariant MathTransform::Input::inputDataData(Role role) const
{
   switch (role)
   {
   case Role::CommandLineName: return QString("in");
   case Role::Title: return tr("Input:");
   case Role::WhatsThis: return tr("Input data object of type Integer Array.");
   case Role::DataType: return DataFactory::IntegerArrayType;
   default: return QVariant();
   }
}






/*!
 *
 * @param role  
 */
QVariant MathTransform::Input::outputDataData(Role role) const
{
   switch (role)
   {
   case Role::CommandLineName: return QString("out");
   case Role::Title: return tr("Output:");
   case Role::WhatsThis: return tr("Output data object of type Integer Array.");
   case Role::DataType: return DataFactory::IntegerArrayType;
   default: return QVariant();
   }
}






/*!
 *
 * @param role  
 */
QVariant MathTransform::Input::operationTypeData(Role role) const
{
   switch (role)
   {
   case Role::CommandLineName: return QString("type");
   case Role::Title: return tr("Operation Type:");
   case Role::WhatsThis: return tr("The type of operation to be done to output values.");
   case Role::Default: return _operationNames.first();
   case Role::SelectionValues: return _operationNames;
   default: return QVariant();
   }
}






/*!
 *
 * @param role  
 */
QVariant MathTransform::Input::amountData(Role role) const
{
   switch (role)
   {
   case Role::CommandLineName: return QString("amount");
   case Role::Title: return tr("Amount:");
   case Role::WhatsThis: return tr("The number used for the given type of operation.");
   case Role::Default: return 0;
   case Role::Minimum: return std::numeric_limits<int>::min();
   case Role::Maximum: return std::numeric_limits<int>::max();
   default: return QVariant();
   }
}
