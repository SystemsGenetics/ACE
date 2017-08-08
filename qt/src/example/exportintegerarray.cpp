#include "exportintegerarray.h"
#include "integerarray.h"
#include "datafactory.h"






EAbstractAnalytic::ArgumentType ExportIntegerArray::getArgumentData(int argument)
{
   switch (argument)
   {
   case InputData:
      return ArgumentType::DataIn;
   case OutputFile:
      return ArgumentType::FileOut;
   default:
      return ArgumentType::Bool;
   }
}






QVariant ExportIntegerArray::getArgumentData(int argument, EAbstractAnalytic::Role role)
{
   switch (role)
   {
   case Role::CommandLineName:
      switch (argument)
      {
      case InputData:
         return QString("in");
      case OutputFile:
         return QString("out");
      default:
         return QString();
      }
   case Role::Title:
      switch (argument)
      {
      case InputData:
         return QString("Input integer array:");
      case OutputFile:
         return QString("Output file:");
      default:
         return QString();
      }
   case Role::FileFilters:
      switch (argument)
      {
      case OutputFile:
         return QString("Raw Text File (*.txt)");
      default:
         return QString();
      }
   case Role::DataType:
      switch (argument)
      {
      case InputData:
         return DataFactory::IntegerArrayType;
      default:
         return QVariant();
      }
   default:
      return QVariant();
   }
}






void ExportIntegerArray::setArgument(int argument, QFile *file)
{
   if ( argument == OutputFile )
   {
      _output = file;
   }
}






void ExportIntegerArray::setArgument(int argument, EAbstractData *data)
{
   if ( argument == InputData )
   {
      _input = dynamic_cast<IntegerArray*>(data);
   }
}






bool ExportIntegerArray::initialize()
{
   return false;
}






bool ExportIntegerArray::runBlock(int block)
{
   Q_UNUSED(block);
   QTextStream stream(_output);
   for (auto i = _input->_numbers.constBegin(); i != _input->_numbers.constEnd() ;++i)
   {
      stream << *i << "\n";
   }
   return false;
}
