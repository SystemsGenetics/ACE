#include "ace_run.h"
#include <core/ace_analytic_abstractmanager.h>
#include <core/eabstractanalyticfactory.h>
#include <core/eexception.h>



using namespace Ace;
//






/*!
 * Constructs a new run object with the given command arguments and options. 
 *
 * @param command The command arguments, including the very first argument that 
 *                should be run, chunkrun, or merge. 
 *
 * @param options The command line options used as input for the analytic being 
 *                ran. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Setup the chunk run indexes and then setup this object's analytic manager. 
 */
Run::Run(const Command& command, const Options& options):
   _stream(stdout),
   _options(options),
   _command(command)
{
   setupIndexes();
   setupManager(getType());
}






/*!
 * Called when this object's analytic manager has made progress to a new percent 
 * complete. 
 *
 * @param percentComplete The new percent complete out of 100 for this object's 
 *                        analytic manager. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Output a new percent complete to standard output, overwriting the previous 
 *    percent. Flush the stream to make sure it updates. 
 */
void Run::progressed(int percentComplete)
{
   _stream << "\r" << QString::number(percentComplete) << "%";
   _stream.flush();
}






/*!
 * This is called when this object's analytic manager is done. This is simply used 
 * to update the percent complete to 100. 
 */
void Run::done()
{
   _stream << "\r100%\n";
}






/*!
 * Called when this object's analytic manager is finished and ready to be deleted. 
 */
void Run::finished()
{
   deleteLater();
}






/*!
 * Configures the chunk indexes, if any, for this analytic run. If this is not a 
 * chunk or merge run then the default index is 0 and default size is 1. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Pop this object's first command argument. If it is a chunk run then setup the 
 *    chunk run, else if it is a merge run then setup the merge run, else it must 
 *    be a regular run so leave the default index and size. 
 *
 * 2. If this object's command argument size is empty then throw an exception. 
 */
void Run::setupIndexes()
{
   QString command {_command.pop()};
   if ( command == QString("chunkrun") )
   {
      setupChunk();
   }
   else if ( command == QString("merge") )
   {
      setupMerge();
   }
   if ( _command.size() < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("No analytic name given, exiting..."));
      throw e;
   }
}






/*!
 * Initializes this analytic run as a chunk run, grabbing the index and size from 
 * this object's command arguments. If the index and/or size given is not valid 
 * then an exception is thrown. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this object's command argument size is less than two then throw an 
 *    exception, else go to the next step. 
 *
 * 2. Get the index value by popping the first command argument and the size value 
 *    by popping the second command argument, setting this object's index and size 
 *    to the given values. If the values fail being read as integers or they are 
 *    invalid then throw an exception. 
 */
void Run::setupChunk()
{
   if ( _command.size() < 2 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Index and size values not given for chunkrun, exiting..."));
      throw e;
   }
   bool ok;
   _index = _command.pop().toInt(&ok);
   if ( !ok )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Given index for chunkrun is invalid, exiting..."));
      throw e;
   }
   _size = _command.pop().toInt(&ok);
   if ( !ok )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Given size for chunkrun is invalid, exiting..."));
      throw e;
   }
   if ( _index < 0 || _size < 0 || _index >= _size )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Given index and/or size is invalid, exiting..."));
      throw e;
   }
}






/*!
 * Initializes this analytic run as a merge run, grabbing the size form this 
 * object's command arguments. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Set this object's index to -1 denoting a merge run. If this object's command 
 *    argument size is empty then throw an exception, else go to the next step. 
 *
 * 2. Get the size by popping the first command argument, setting it to this 
 *    object's size. If reading the size as an integer fails then throw an 
 *    exception. 
 */
void Run::setupMerge()
{
   _index = -1;
   if ( _command.size() < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Size value not given for merge, exiting..."));
      throw e;
   }
   bool ok;
   _size = _command.pop().toInt(&ok);
   if ( !ok )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Given size for merge is invalid, exiting..."));
      throw e;
   }
}






/*!
 * Returns the analytic type based off the name given from this object's first 
 * command argument. If no analytic can be found with the given command line name 
 * then an exception is thrown. 
 *
 * @return The analytic type that matches the name given by this object's first 
 *         command argument. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Populate a string list of all analytic command names so that their indexes 
 *    match their integer type. 
 *
 * 2. Pop this object's first command argument and attempt to find it in the list 
 *    of analytic command names. If the analytic is not found in the list then 
 *    throw an exception, else return the analytic type found. 
 */
quint16 Run::getType()
{
   QStringList commandNames;
   EAbstractAnalyticFactory& factory {EAbstractAnalyticFactory::instance()};
   for (quint16 i = 0; i < factory.size() ;++i)
   {
      commandNames << factory.commandName(i);
   }
   QString name {_command.pop()};
   int type {commandNames.indexOf(name)};
   if ( type < 0 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Unknown analytic '%1'; no analytic exists by that name.").arg(name));
      throw e;
   }
   return static_cast<quint16>(type);
}






/*!
 * Creates a new analytic manager with a given analytic type and this object's size 
 * and index, setting this object as its parent. This also connects all relevant 
 * signals to this object's slots, sets all analytic arguments for this object's 
 * options, and initializes the manager for execution. 
 *
 * @param type  
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a new analytic manager with the given analytic type and this object's 
 *    index and size, setting this as its parent. 
 *
 * 2. Connect all signals to slots, add all arguments to the analytic manager, and 
 *    then initialize the manager for execution. 
 */
void Run::setupManager(quint16 type)
{
   _manager = Analytic::AbstractManager::makeManager(type,_index,_size).release();
   _manager->setParent(this);
   connect(_manager,&Analytic::AbstractManager::progressed,this,&Run::progressed);
   connect(_manager,&Analytic::AbstractManager::done,this,&Run::done);
   connect(_manager,&Analytic::AbstractManager::finished,this,&Run::finished);
   addArguments();
   _manager->initialize();
}






/*!
 * Parses this object's options and sets them to this object's analytic manager as 
 * analytic arguments. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Iterate through all arguments for this object's analytic manager for the 
 *    following steps. 
 *
 * 2. Get the command line name for the analytic argument. 
 *
 * 3. If the analytic argument type is integer, double, or selection then pass the 
 *    argument index and its command line name to special methods that handle those 
 *    types. Else simply set the analytic argument by attempting to find one of 
 *    this object's options whose key equals this argument's command line name, 
 *    defaulting to the default value if not found. 
 */
void Run::addArguments()
{
   for (int i = 0; i < _manager->size() ;++i)
   {
      QString argumentName
      {//
         _manager->data(i,EAbstractAnalytic::Input::Role::CommandLineName).toString()
      };
      switch (_manager->type(i))
      {
      case EAbstractAnalytic::Input::Type::Boolean:
      case EAbstractAnalytic::Input::Type::String:
      case EAbstractAnalytic::Input::Type::FileIn:
      case EAbstractAnalytic::Input::Type::FileOut:
      case EAbstractAnalytic::Input::Type::DataIn:
      case EAbstractAnalytic::Input::Type::DataOut:
         _manager->set(i,_options.find(argumentName));
         break;
      case EAbstractAnalytic::Input::Type::Integer:
         addInteger(i,argumentName);
         break;
      case EAbstractAnalytic::Input::Type::Double:
         addDouble(i,argumentName);
         break;
      case EAbstractAnalytic::Input::Type::Selection:
         addSelection(i,argumentName);
         break;
      }
}






/*!
 * Sets an analytic argument with the given index to one of this object's options 
 * with the given key as an integer. If the option's value cannot be converted to 
 * an integer or it is out of bounds then an exception is thrown. 
 *
 * @param index The analytic argument index whose value is set with an option with 
 *              the given key. 
 *
 * @param key The key used to find the option used to set the analytic argument. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Find an option with the given key, converting it to an integer value. If 
 *    converting it to an integer fails then throw an exception, else go to the 
 *    next step. 
 *
 * 2. If the integer value is less than the minimum or greater than the maximum 
 *    then throw an exception, else set the analytic argument with the given index 
 *    to the integer value. 
 */
void Run::addInteger(int index, const QString& key)
{
   bool ok;
   int value {_options.find(key).toInt(&ok)};
   if ( !ok )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Integer argument '%1' is not a valid integer.").arg(key));
      throw e;
   }
   int minimum {_manager->data(index,EAbstractAnalytic::Input::Role::Minimum).toInt()};
   int maximum {_manager->data(index,EAbstractAnalytic::Input::Role::Maximum).toInt()};
   if ( value < minimum || value > maximum )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Integer argument '%1' cannot be set to %2 (%3-%4 is valid range).")
                   .arg(key)
                   .arg(value)
                   .arg(minimum)
                   .arg(maximum));
      throw e;
   }
   _manager->set(index,value);
}






/*!
 * Sets an analytic argument with the given index to one of this object's options 
 * with the given key as a double. If the option's value cannot be converted to a 
 * double or it is out of bounds then an exception is thrown. 
 *
 * @param index The analytic argument index whose value is set with an option with 
 *              the given key. 
 *
 * @param key The key used to find the option used to set the analytic argument. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Find an option with the given key, converting it to a double value. If 
 *    converting it to a double fails then throw an exception, else go to the next 
 *    step. 
 *
 * 2. If the double value is less than the minimum or greater than the maximum then 
 *    throw an exception, else set the analytic argument with the given index to 
 *    the double value. 
 */
void Run::addDouble(int index, const QString& key)
{
   bool ok;
   double value {_options.find(key).toDouble(&ok)};
   if ( !ok )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Floating point argument '%1' is not a valid integer.").arg(key));
      throw e;
   }
   double minimum {_manager->data(index,EAbstractAnalytic::Input::Role::Minimum).toDouble()};
   double maximum {_manager->data(index,EAbstractAnalytic::Input::Role::Maximum).toDouble()};
   if ( value < minimum || value > maximum )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Floating point argument '%1' cannot be set to %2 (%3-%4 is valid range).")
                   .arg(key)
                   .arg(value)
                   .arg(minimum)
                   .arg(maximum));
      throw e;
   }
   _manager->set(index,value);
}






/*!
 * Sets an analytic argument with the given index to one of this object's options 
 * with the given key as a selection. If the option's value is not a valid 
 * selection then an exception is thrown. 
 *
 * @param index The analytic argument index whose value is set with an option with 
 *              the given key. 
 *
 * @param key The key used to find the option used to set the analytic argument. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Find an option with the given key, setting it to the analytic argument with 
 *    the given index. If the value of the option found is not a valid selection 
 *    option for the analytic argument with the given index then throw an 
 *    exception. 
 */
void Run::addSelection(int index, const QString& key)
{
   QString value {_options.find(key)};
   QStringList values
   {//
      _manager->data(index,EAbstractAnalytic::Input::Role::SelectionValues).toStringList()
}






/*! !!! UNKNOWN FUNCTION !!! */
.arg(key)
.arg(value));

{

dex,value);


}
