#include "analytic.h"
#include "data.h"

#define ADDITION "Addition"
#define SUBTRACTION "Subtraction"
#define MULTIPLICATION "Multiplication"
#define DIVISION "Division"






int Analytic::getArgumentCount()
{
   return Total;
}






QString Analytic::getCommandLineName(int argument)
{
   switch (argument)
   {
   case InputData:
      return QString("in");
   case OutputData:
      return QString("out");
   case OperationType:
      return QString("type");
   case Amount:
      return QString("amount");
   default:
      return QString();
   }
}






EAbstractAnalytic::ArgumentType Analytic::getArgumentType(int argument)
{
   using Type = EAbstractAnalytic::ArgumentType;
   switch (argument)
   {
   case InputData:
      return Type::DataObjectIn;
   case OutputData:
      return Type::DataObjectOut;
   case OperationType:
      return Type::Combo;
   case Amount:
      return Type::Integer;
   default:
      return Type::Bool;
   }
}






QString Analytic::getArgumentTitle(int argument)
{
   switch (argument)
   {
   case InputData:
      return QString("Input:");
   case OutputData:
      return QString("Output:");
   case OperationType:
      return QString("Operation Type:");
   case Amount:
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
      return QStringList() << ADDITION << SUBTRACTION << MULTIPLICATION << DIVISION;
   default:
      return QStringList();
   }
}






QString Analytic::getArgumentWhatsThis(int argument)
{
   switch (argument)
   {
   case InputData:
      return QString("Input data object of type Integer Array.");
   case OutputData:
      return QString("Output data object of type Integer Array.");
   case OperationType:
      return QString("The type of operation to be done to output values.");
   case Amount:
      return QString("The number used for the given type of operation.");
   default:
      return QString();
   }
}






QStringList Analytic::getFileArgumentFilters(int /*argument*/)
{
   return QStringList();
}






quint16 Analytic::getDataArgumentType(int /*argument*/)
{
   return 0;
}






void Analytic::setArgument(int argument, QVariant value)
{
   switch (argument)
   {
   case OperationType:
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
      _amount = value.toInt();
      break;
   }
}






void Analytic::setArgument(int /*argument*/, QFile* /*file*/)
{}






void Analytic::setArgument(int argument, EAbstractData* data)
{
   switch (argument)
   {
   case InputData:
      _in = dynamic_cast<Data*>(data);
      break;
   case OutputData:
      _out = dynamic_cast<Data*>(data);
      break;
   }
}






bool Analytic::initialize()
{
   _out->_numbers.clear();
   _out->_numbers.reserve(_in->_numbers.size());
   _out->_futureSize = _in->_numbers.size();
   return true;
}






int Analytic::getBlockSize()
{
   return 1;
}






bool Analytic::runBlock(int /*block*/)
{
   int total {_in->_numbers.size()};
   int done {0};
   int lastPercent {0};
   for (auto i = _in->_numbers.begin(); i != _in->_numbers.end() ;++i)
   {
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
      ++done;
      int newPercent = 100*total/done;
      if ( newPercent != lastPercent )
      {
         lastPercent = newPercent;
         emit progressed(newPercent);
      }
   }
   return false;
}






void Analytic::finish()
{
   emit finished();
}
