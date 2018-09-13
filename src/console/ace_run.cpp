#include "ace_run.h"
#include "../core/ace_analytic_abstractmanager.h"
#include "../core/eabstractanalyticfactory.h"
#include "../core/eexception.h"
#include "../core/edebug.h"
#include "../core/ace_logserver.h"
#include "../core/ace_settings.h"
#include "../core/ace_qmpi.h"



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
 */
Run::Run(const Command& command, const Options& options):
   _stream(stdout),
   _options(options),
   _command(command)
{
   EDEBUG_FUNC(this,&command,&options)

   // Setup the chunk run indexes and then setup this object's analytic manager. 
   setupIndexes();
   setupManager(getType());
}






/*!
 * Called when this object's analytic manager has made progress to a new percent 
 * complete. 
 *
 * @param percentComplete The new percent complete out of 100 for this object's 
 *                        analytic manager. 
 */
void Run::progressed(int percentComplete)
{
   EDEBUG_FUNC(this,percentComplete)

   // Output a new percent complete to standard output, overwriting the previous 
   // percent. Flush the stream to make sure it updates. 
   _stream << QString::number(percentComplete) << "%\n";
   _stream.flush();
}






/*!
 * This is called when this object's analytic manager is done. This is simply used 
 * to update the percent complete to 100. 
 */
void Run::done()
{
   EDEBUG_FUNC(this)

   _stream << "100%\n";
}






/*!
 * Called when this object's analytic manager is finished and ready to be deleted. 
 */
void Run::finished()
{
   EDEBUG_FUNC(this)

   deleteLater();
}






/*!
 * Configures the chunk indexes, if any, for this analytic run. If this is not a 
 * chunk or merge run then the default index is 0 and default size is 1. 
 */
void Run::setupIndexes()
{
   EDEBUG_FUNC(this)

   // Pop this object's first command argument. If it is a chunk run then setup the 
   // chunk run, else if it is a merge run then setup the merge run, else it must be 
   // a regular run so leave the default index and size. 
   QString command {_command.pop()};
   if ( command == QString("chunkrun") )
   {
      setupChunk();
   }
   else if ( command == QString("merge") )
   {
      setupMerge();
   }

   // If this object's command argument size is empty then throw an exception. 
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
 */
void Run::setupChunk()
{
   EDEBUG_FUNC(this)

   // If this object's command argument size is less than two then throw an 
   // exception, else go to the next step. 
   if ( _command.size() < 2 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Index and size values not given for chunkrun, exiting..."));
      throw e;
   }

   // Get the index value by popping the first command argument and the size value by 
   // popping the second command argument, setting this object's index and size to 
   // the given values. If the values fail being read as integers or they are invalid 
   // then throw an exception. 
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
 */
void Run::setupMerge()
{
   EDEBUG_FUNC(this)

   // Set this object's index to -1 denoting a merge run. If this object's command 
   // argument size is empty then throw an exception, else go to the next step. 
   _index = -1;
   if ( _command.size() < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Size value not given for merge, exiting..."));
      throw e;
   }

   // Get the size by popping the first command argument, setting it to this object's 
   // size. If reading the size as an integer fails then throw an exception. 
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
 */
quint16 Run::getType()
{
   EDEBUG_FUNC(this)

   // Populate a string list of all analytic command names so that their indexes 
   // match their integer type. 
   QStringList commandNames;
   EAbstractAnalyticFactory& factory {EAbstractAnalyticFactory::instance()};
   for (quint16 i = 0; i < factory.size() ;++i)
   {
      commandNames << factory.commandName(i);
   }

   // Pop this object's first command argument and attempt to find it in the list of 
   // analytic command names. If the analytic is not found in the list then throw an 
   // exception, else return the analytic type found. 
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
 */
void Run::setupManager(quint16 type)
{
   EDEBUG_FUNC(this,type)

   // Create a new analytic manager with the given analytic type and this object's 
   // index and size, setting this as its parent. 
   _manager = Analytic::AbstractManager::makeManager(type,_index,_size).release();
   _manager->setParent(this);

   // Connect all signals to slots, add all arguments to the analytic manager, and 
   // then initialize the manager for execution. 
   connect(_manager,&Analytic::AbstractManager::progressed,this,&Run::progressed);
   connect(_manager,&Analytic::AbstractManager::done,this,&Run::done);
   connect(_manager,&Analytic::AbstractManager::finished,this,&Run::finished);
   addArguments();

   // Initialize the analytic manager immediately. 
   _manager->initialize();
}






/*!
 * Parses this object's options and sets them to this object's analytic manager as 
 * analytic arguments. 
 */
void Run::addArguments()
{
   EDEBUG_FUNC(this)

   // Iterate through all arguments for this object's analytic manager for the 
   // following steps. 
   for (int i = 0; i < _manager->size() ;++i)
   {
      // Get the command line name for the analytic argument. 
      QString argumentName
      {
         _manager->data(i,EAbstractAnalytic::Input::Role::CommandLineName).toString()
      };

      // If the command line name does not exist in this object's options then skip to 
      // the next analytic argument. 
      if ( !_options.contains(argumentName) )
      {
         continue;
      }

      // If the analytic argument type is integer, double, or selection then pass the 
      // argument index and its command line name to special methods that handle those 
      // types. Else simply set the analytic argument by attempting to find one of this 
      // object's options whose key equals this argument's command line name, defaulting 
      // to the default value if not found. 
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
 */
void Run::addInteger(int index, const QString& key)
{
   EDEBUG_FUNC(this,index,key)

   // Find an option with the given key, converting it to an integer value. If 
   // converting it to an integer fails then throw an exception, else go to the next 
   // step. 
   bool ok;
   int value {_options.find(key).toInt(&ok)};
   if ( !ok )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Integer argument '%1' is not a valid integer.").arg(key));
      throw e;
   }

   // If the integer value is less than the minimum or greater than the maximum then 
   // throw an exception, else set the analytic argument with the given index to the 
   // integer value. 
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
 */
void Run::addDouble(int index, const QString& key)
{
   EDEBUG_FUNC(this,index,key)

   // Find an option with the given key, converting it to a double value. If 
   // converting it to a double fails then throw an exception, else go to the next 
   // step. 
   bool ok;
   double value {_options.find(key).toDouble(&ok)};
   if ( !ok )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Floating point argument '%1' is not a valid integer.").arg(key));
      throw e;
   }

   // If the double value is less than the minimum or greater than the maximum then 
   // throw an exception, else set the analytic argument with the given index to the 
   // double value. 
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
 */
void Run::addSelection(int index, const QString& key)
{
   EDEBUG_FUNC(this,index,key)

   // Find an option with the given key, setting it to the analytic argument with the 
   // given index. If the value of the option found is not a valid selection option 
   // for the analytic argument with the given index then throw an exception. 
   QString value {_options.find(key)};
   QStringList values
   {
      _manager->data(index,EAbstractAnalytic::Input::Role::SelectionValues).toStringList()
   };
   if ( !values.contains(value) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Selection argument '%1' does not contain '%2' as a valid option.")
                   .arg(key)
                   .arg(value));
      throw e;
   }
   _manager->set(index,value);
}
