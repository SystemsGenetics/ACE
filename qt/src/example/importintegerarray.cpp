#include "importintegerarray.h"
#include "integerarray.h"
#include "datafactory.h"






EAbstractAnalytic::ArgumentType ImportIntegerArray::getArgumentData(int argument)
{
   // determine which argument is being queried
   switch (argument)
   {
   case InputFile:
      // this is input file argument
      return ArgumentType::FileIn;
   case OutputData:
      // this is output data argument
      return ArgumentType::DataOut;
   default:
      // unknown argument
      return ArgumentType::Bool;
   }
}






QVariant ImportIntegerArray::getArgumentData(int argument, EAbstractAnalytic::Role role)
{
   // determine which role is being queried
   switch (role)
   {
   case Role::CommandLineName:
      // determine which argument is being queried
      switch (argument)
      {
      case InputFile:
         // this is input file argument
         return QString("in");
      case OutputData:
         // this is output data argument
         return QString("out");
      default:
         // unknown argument
         return QString();
      }
   case Role::Title:
      // determine which argument is being queried
      switch (argument)
      {
      case InputFile:
         // this is input file argument
         return QString("Input Text File:");
      case OutputData:
         // this is output data argument
         return QString("Output integer array:");
      default:
         // unknown argument
         return QString();
      }
   case Role::WhatsThis:
      // determine which argument is being queried
      switch (argument)
      {
      case InputFile:
         // this is input file argument
         return QString("Raw text file that contains a list of integers to import.");
      case OutputData:
         // this is output data argument
         return QString("New integer array that will contain imported integers.");
      default:
         // unknown argument
         return QString();
      }
   case Role::FileFilters:
      // determine which argument is being queried
      switch (argument)
      {
      case InputFile:
         // this is input file argument
         return QString("Raw Text File (*.txt)");
      default:
         // unknown argument
         return QString();
      }
   case Role::DataType:
      // determine which argument is being queried
      switch (argument)
      {
      case OutputData:
         // this is output data argument
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






void ImportIntegerArray::setArgument(int argument, QFile *file)
{
   // if argument is input file set it
   if ( argument == InputFile )
   {
      _input = file;
   }
}






void ImportIntegerArray::setArgument(int argument, EAbstractData *data)
{
   // if argument is output data set it
   if ( argument == OutputData )
   {
      _output = dynamic_cast<IntegerArray*>(data);
   }
}






bool ImportIntegerArray::initialize()
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

   // clear the output data object and do not pre-allocate by returning false
   _output->_numbers.clear();
   return false;
}






bool ImportIntegerArray::runBlock(int block)
{
   // do not use block because there is only one
   Q_UNUSED(block);

   // open input file and read all integers, adding them to output integer array as they are being
   // read
   QTextStream stream(_input);
   int value;
   stream >> value;
   while ( stream.status() == QTextStream::Ok )
   {
      _output->_numbers.append(value);
      stream >> value;
   }

   // signal block is finished with execution
   return false;
}
