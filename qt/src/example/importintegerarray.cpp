#include "importintegerarray.h"
#include "integerarray.h"
#include "datafactory.h"






EAbstractAnalytic::ArgumentType ImportIntegerArray::getArgumentData(int argument)
{
   switch (argument)
   {
   case InputFile:
      return ArgumentType::FileIn;
   case OutputData:
      return ArgumentType::DataOut;
   default:
      return ArgumentType::Bool;
   }
}






QVariant ImportIntegerArray::getArgumentData(int argument, EAbstractAnalytic::Role role)
{
   switch (role)
   {
   case Role::CommandLineName:
      switch (argument)
      {
      case InputFile:
         return QString("in");
      case OutputData:
         return QString("out");
      default:
         return QString();
      }
   case Role::Title:
      switch (argument)
      {
      case InputFile:
         return QString("Input Text File:");
      case OutputData:
         return QString("Output integer array:");
      default:
         return QString();
      }
   case Role::FileFilters:
      switch (argument)
      {
      case InputFile:
         return QString("Raw Text File (*.txt)");
      default:
         return QString();
      }
   case Role::DataType:
      switch (argument)
      {
      case OutputData:
         return DataFactory::IntegerArrayType;
      default:
         return QVariant();
      }
   default:
      return QVariant();
   }
}






void ImportIntegerArray::setArgument(int argument, QFile *file)
{
   if ( argument == InputFile )
   {
      _input = file;
   }
}






void ImportIntegerArray::setArgument(int argument, EAbstractData *data)
{
   if ( argument == OutputData )
   {
      _output = dynamic_cast<IntegerArray*>(data);
   }
}






bool ImportIntegerArray::initialize()
{
   _output->_numbers.clear();
   return false;
}






bool ImportIntegerArray::runBlock(int block)
{
   Q_UNUSED(block);
   QTextStream stream(_input);
   int value;
   stream >> value;
   while ( stream.status() == QTextStream::Ok )
   {
      _output->_numbers.append(value);
      stream >> value;
   }
   return false;
}
