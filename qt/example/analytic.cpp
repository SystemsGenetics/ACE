#include "analytic.h"
#include "data.h"
#include "datafactory.h"

#define ADDITION "Addition"
#define SUBTRACTION "Subtraction"
#define MULTIPLICATION "Multiplication"
#define DIVISION "Division"






int Analytic::getArgumentCount()
{
   // Return total number of arguments
   return Total;
}






QString Analytic::getCommandLineName(int argument)
{
   switch (argument)
   {
   case InputData:
      // This is input data, return string
      return QString("in");
   case OutputData:
      // This is output data, return string
      return QString("out");
   case OperationType:
      // This is operation type, return string
      return QString("type");
   case Amount:
      // This is amount, return string
      return QString("amount");
   default:
      // Unknown argument, return empty string
      return QString();
   }
}






EAbstractAnalytic::ArgumentType Analytic::getArgumentType(int argument)
{
   using Type = EAbstractAnalytic::ArgumentType;
   switch (argument)
   {
   case InputData:
      // This is input data, return DataIn type
      return Type::DataIn;
   case OutputData:
      // This is output data, return DataOut type
      return Type::DataOut;
   case OperationType:
      // This is operation type, return combo box type
      return Type::Combo;
   case Amount:
      // This is amount, return integer type
      return Type::Integer;
   default:
      // Unknown argument, return base bool type
      return Type::Bool;
   }
}






QString Analytic::getArgumentTitle(int argument)
{
   switch (argument)
   {
   case InputData:
      // This is input data, return title
      return QString("Input:");
   case OutputData:
      // This is output data, return title
      return QString("Output:");
   case OperationType:
      // This is operation type, return title
      return QString("Operation Type:");
   case Amount:
      // This is amount, return title
      return QString("Amount:");
   default:
      return QString();
   }
}






QStringList Analytic::getComboValues(int argument)
{
   switch (argument)
   {
   case OperationType:
      // This is operation type, return list of combo box options
      return QStringList() << ADDITION << SUBTRACTION << MULTIPLICATION << DIVISION;
   default:
      // Any other argument is not combo box so return empty list
      return QStringList();
   }
}






QString Analytic::getArgumentWhatsThis(int argument)
{
   switch (argument)
   {
   case InputData:
      // This is input data, return help
      return QString("Input data object of type Integer Array.");
   case OutputData:
      // This is output data, return help
      return QString("Output data object of type Integer Array.");
   case OperationType:
      // This is operation type, return help
      return QString("The type of operation to be done to output values.");
   case Amount:
      // This is amount, return help
      return QString("The number used for the given type of operation.");
   default:
      // Unknown argument, return empty string
      return QString();
   }
}






QStringList Analytic::getFileArgumentFilters(int /*argument*/)
{
   // This analytic uses no files arguments, always return empty list
   return QStringList();
}






quint16 Analytic::getDataArgumentType(int /*argument*/)
{
   // Because this analytic uses only one data type, always return that type identifier
   return DataFactory::Integers;
}






void Analytic::setArgument(int argument, QVariant value)
{
   switch (argument)
   {
   case OperationType:
      // This is operation type, set operation type to new value
      {
         QString argument = value.toString();
         if ( argument == ADDITION )
         {
            _type = OperType::Addition;
         }
         else if ( argument == SUBTRACTION )
         {
            _type = OperType::Subtraction;
         }
         else if ( argument == MULTIPLICATION )
         {
            _type = OperType::Multiplication;
         }
         else if ( argument == DIVISION )
         {
            _type = OperType::Division;
         }
         break;
      }
   case Amount:
      // This is amount, set amount to new value
      _amount = value.toInt();
      break;
   }
}






void Analytic::setArgument(int /*argument*/, QFile* /*file*/)
{
   // This analytic has no file arguments so this virtual function does nothing
}






void Analytic::setArgument(int argument, EAbstractData* data)
{
   switch (argument)
   {
   case InputData:
      // This is input data, set reference
      _in = dynamic_cast<Data*>(data);
      break;
   case OutputData:
      // This is output data, set reference
      _out = dynamic_cast<Data*>(data);
      break;
   }
}






bool Analytic::initialize()
{
   // Clear any numbers the output data may hold
   _out->_numbers.clear();

   // Reserve the amount of numbers output data will hold
   _out->_numbers.reserve(_in->_numbers.size());
   _out->_futureSize = _in->_numbers.size();

   // Return true so the output data object pre-allocates file space
   return true;
}






int Analytic::getBlockSize()
{
   // Because this is a simple analytic simply return 1
   return 1;
}






bool Analytic::runBlock(int /*block*/)
{
   // Get total amount of numbers to process
   int total {_in->_numbers.size()};

   // Initialize variables for keeping track of progress
   int done {0};
   int lastPercent {0};

   // Increment through all numbers of input data
   for (auto i = _in->_numbers.begin(); i != _in->_numbers.end() ;++i)
   {
      // Do mathematical operation with input data value and push to end of output data
      switch (_type)
      {
      case OperType::Addition:
         _out->_numbers.push_back((*i)+_amount);
         break;
      case OperType::Subtraction:
         _out->_numbers.push_back((*i)-_amount);
         break;
      case OperType::Multiplication:
         _out->_numbers.push_back((*i)*_amount);
         break;
      case OperType::Division:
         _out->_numbers.push_back((*i)/_amount);
         break;
      }

      // Increment done counter and calculate new percentage complete
      ++done;
      int newPercent = 100*total/done;

      // If percentage complete has changed emit progressed(int) signal
      if ( newPercent != lastPercent )
      {
         lastPercent = newPercent;
         emit progressed(newPercent);
      }
   }

   // Because this is a simple analytic the one block it runs only does so once, so return false
   // to signal to ACE this block is done with execution
   return false;
}






void Analytic::finish()
{
   // Emit the finished signal
   emit finished();
}
