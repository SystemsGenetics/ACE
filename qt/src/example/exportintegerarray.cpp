#include "exportintegerarray.h"
#include "integerarray.h"
#include "datafactory.h"






EAbstractAnalytic::ArgumentType ExportIntegerArray::getArgumentData(int argument)
{
   // determine which argument is being queried
   switch (argument)
   {
   case InputData:
      // this is input data argument
      return ArgumentType::DataIn;
   case OutputFile:
      // this is output file argument
      return ArgumentType::FileOut;
   default:
      // unkonwn argument
      return ArgumentType::Bool;
   }
}






QVariant ExportIntegerArray::getArgumentData(int argument, EAbstractAnalytic::Role role)
{
   // determine which role is being queried
   switch (role)
   {
   case Role::CommandLineName:
      // determine which argument is being queried
      switch (argument)
      {
      case InputData:
         // this is input data argument
         return QString("in");
      case OutputFile:
         // this is output file argument
         return QString("out");
      default:
         // unknown argument
         return QString();
      }
   case Role::Title:
      // determine which argument is being queried
      switch (argument)
      {
      case InputData:
         // this is input data argument
         return QString("Input integer array:");
      case OutputFile:
         // this is output file argument
         return QString("Output file:");
      default:
         // unknown argument
         return QString();
      }
   case Role::WhatsThis:
      // determine which argument is being queried
      switch (argument)
      {
      case InputData:
         // this is input data argument
         return QString("Integer array that will have its integers exported to raw text file.");
      case OutputFile:
         // this is output file argument
         return QString("Raw text file that will be overwritten with input data's integers.");
      default:
         // unknown argument
         return QString();
      }
   case Role::FileFilters:
      // determine which argument is being queried
      switch (argument)
      {
      case OutputFile:
         // this is output file argument
         return QString("Raw Text File (*.txt)");
      default:
         // unknown argument
         return QString();
      }
   case Role::DataType:
      // determine which argument is being queried
      switch (argument)
      {
      case InputData:
         // this is input data type
         return DataFactory::IntegerArrayType;
      default:
         // unknown argument
         return QVariant();
      }
   default:
      // unknown role
      return QVariant();
   }
}






void ExportIntegerArray::setArgument(int argument, QFile *file)
{
   // if this is output file argument set it
   if ( argument == OutputFile )
   {
      _output = file;
   }
}






void ExportIntegerArray::setArgument(int argument, EAbstractData *data)
{
   // if this is input data argument set it
   if ( argument == InputData )
   {
      _input = dynamic_cast<IntegerArray*>(data);
   }
}






bool ExportIntegerArray::initialize()
{
   // make sure we have valid inputs and outputs
   if ( !_input || !_output )
   {
      // If failure occured create exception to report failure
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Argument Error"));
      e.setDetails(QObject::tr("Did not get valid input and/or output arguments."));
      throw e;
   }

   // no initialization for simple analytic
   return false;
}






bool ExportIntegerArray::runBlock(int block)
{
   // block is unused
   Q_UNUSED(block);

   // iterate through input data and write out all integers to output file
   QTextStream stream(_output);
   for (auto i = _input->_numbers.constBegin(); i != _input->_numbers.constEnd() ;++i)
   {
      stream << *i << "\n";
   }

   // return that this block is finished
   return false;
}
