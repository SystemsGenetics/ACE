#include "ace_run.h"
#include "../core/ace_analytic_abstractmanager.h"
#include "../core/eabstractanalyticfactory.h"
#include "../core/eexception.h"
#include "../core/edebug.h"
#include "../core/ace_logserver.h"
#include "../core/ace_settings.h"
#include "../core/ace_qmpi.h"



namespace Ace
{






/*!
 * Constructs a new run object with the given command arguments and options.
 *
 * @param command The command arguments, including the very first argument that
 *                should be run, chunkrun, or merge.
 *
 * @param options The command line options used as input for the analytic being
 *                ran.
 */
Run::Run(const Command& command, const Options& options)
   :
   _stream(stdout),
   _options(options),
   _command(command)
{
   // Add the debug header.
   EDEBUG_FUNC(this,&command,&options);

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
   // Add the debug header.
   EDEBUG_FUNC(this,percentComplete);

   // Output a new percent complete to standard output, flushing the stream to make
   // sure it updates.
   _stream << QString::number(percentComplete) << "%\n";
   _stream.flush();
}






/*!
 * This is called when this object's analytic manager is done. This is simply
 * used to update the percent complete to 100.
 */
void Run::done()
{
   EDEBUG_FUNC(this);
   _stream << "100%\n";
}






/*!
 * Called when this object's analytic manager is finished and ready to be
 * deleted.
 */
void Run::finished()
{
   EDEBUG_FUNC(this);
   deleteLater();
}






/*!
 * Configures the chunk indexes, if any, for this analytic run. If this is not a
 * chunk or merge run then the default index is 0 and default size is 1.
 */
void Run::setupIndexes()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Pop this object's first command argument to test it for any chunk commands or
   // simply removing it if it is a basic run command.
   QString command {_command.pop()};

   // If the command is chunk run then setup the chunk run.
   if ( command == QString("chunkrun") )
   {
      setupChunk();
   }

   // Else if the command is merge then setup the merge run.
   else if ( command == QString("merge") )
   {
      setupMerge();
   }

   // Make sure there is an additional command argument naming the analytic to run
   // regardless of the type of run.
   if ( _command.size() < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("No analytic name given, exiting..."));
      throw e;
   }
}






/*!
 * Initializes this analytic run as a chunk run, grabbing the index and size
 * from this object's command arguments. If the index and/or size given is not
 * valid then an exception is thrown.
 */
void Run::setupChunk()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Make sure there are two command arguments to be processed.
   if ( _command.size() < 2 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Index and size values not given for chunkrun, exiting..."));
      throw e;
   }

   // Pop and read the chunk index argument and make sure it worked.
   bool ok;
   _index = _command.pop().toInt(&ok);
   if ( !ok )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Given index for chunkrun is invalid, exiting..."));
      throw e;
   }

   // Pop and read the chunk size argument and make sure it worked.
   _size = _command.pop().toInt(&ok);
   if ( !ok )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Given size for chunkrun is invalid, exiting..."));
      throw e;
   }

   // Make sure the index and size arguments given are valid.
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
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Set this object's index to -1 denoting a merge run.
   _index = -1;

   // Make sure there is a command argument to be processed.
   if ( _command.size() < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Size value not given for merge, exiting..."));
      throw e;
   }

   // Pop and read in the size argument, making sure it worked.
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
 * command argument. If no analytic can be found with the given command line
 * name then an exception is thrown.
 *
 * @return The analytic type that matches the name given by this object's first
 *         command argument.
 */
quint16 Run::getType()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Create a string list used to hold all command line names of available
   // analytics.
   QStringList commandNames;

   // Populate a string list with all analytic command names so that their indexes
   // match their integer type.
   EAbstractAnalyticFactory& factory {EAbstractAnalyticFactory::instance()};
   for (quint16 i = 0; i < factory.size() ;++i)
   {
      commandNames << factory.commandName(i);
   }

   // Pop this object's first command argument and attempt to find it in the list of
   // analytic command names, making sure the found index is a valid unsigned 16 bit
   // integer.
   QString name {_command.pop()};
   int type {commandNames.indexOf(name)};
   if ( type < 0 || type > 0xffff )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Unknown analytic '%1'; no analytic exists by that name.").arg(name));
      throw e;
   }

   // Return the found analytic type.
   return static_cast<quint16>(type);
}






/*!
 * Creates a new analytic manager with a given analytic type and this object's
 * size and index, setting this object as its parent. This also connects all
 * relevant signals to this object's slots, sets all analytic arguments for this
 * object's options, and initializes the manager for execution.
 *
 */
void Run::setupManager(quint16 type)
{
   // Add the debug header.
   EDEBUG_FUNC(this,type);

   // Create a new analytic manager with the given analytic type and this object's
   // index and size, setting this as its parent.
   _manager = Analytic::AbstractManager::makeManager(type,_index,_size).release();
   _manager->setParent(this);

   // Connect all required signals to this run object.
   connect(_manager,&Analytic::AbstractManager::progressed,this,&Run::progressed);
   connect(_manager,&Analytic::AbstractManager::done,this,&Run::done);
   connect(_manager,&Analytic::AbstractManager::finished,this,&Run::finished);

   // Set all input arguments to the analytic manager in turn setting the analytic
   // input.
   addArguments();

   // Initialize the analytic manager immediately.
   _manager->initialize();
}






/*!
 * Parses this object's options and sets them to this object's analytic manager
 * as analytic arguments.
 */
void Run::addArguments()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Iterate through all options passed to this run object.
   const QList<QString> arguments {_manager->commandLineArguments()};
   for (int i = 0; i < _options.size() ;++i)
   {
      // Make sure the given option is valid for this run object's analytic.
      if ( !arguments.contains(_options.key(i)) )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(tr("Invalid Option"));
         e.setDetails(tr("Unknown analytic option '%1'.").arg(_options.key(i)));
         throw e;
      }
   }

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
 * Sets an analytic argument with the given index to one of this object's
 * options with the given key as an integer. If the option's value cannot be
 * converted to an integer or it is out of bounds then an exception is thrown.
 *
 * @param index The analytic argument index whose value is set with an option
 *              with the given key.
 *
 * @param key The key used to find the option used to set the analytic argument.
 */
void Run::addInteger(int index, const QString& key)
{
   // Add the debug header.
   EDEBUG_FUNC(this,index,key);

   // Grab the option value with the given key and convert it to an integer, making
   // sure it worked.
   bool ok;
   int value {_options.find(key).toInt(&ok)};
   if ( !ok )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Integer argument '%1' is not a valid integer.").arg(key));
      throw e;
   }

   // Make sure the integer value is within the minimum and maximum bounds for the
   // argument.
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

   // Set the analytic argument with the given index to the valid integer value.
   _manager->set(index,value);
}






/*!
 * Sets an analytic argument with the given index to one of this object's
 * options with the given key as a double. If the option's value cannot be
 * converted to a double or it is out of bounds then an exception is thrown.
 *
 * @param index The analytic argument index whose value is set with an option
 *              with the given key.
 *
 * @param key The key used to find the option used to set the analytic argument.
 */
void Run::addDouble(int index, const QString& key)
{
   // Add the debug header.
   EDEBUG_FUNC(this,index,key);

   // Grab the option value with the given key and convert it to a double floating
   // point, making sure it worked.
   bool ok;
   double value {_options.find(key).toDouble(&ok)};
   if ( !ok )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Floating point argument '%1' is not a valid integer.").arg(key));
      throw e;
   }

   // Make sure the double floating point value is within the minimum and maximum
   // bounds for the given argument.
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

   // Set the analytic argument with the given index to the valid double floating
   // point value.
   _manager->set(index,value);
}






/*!
 * Sets an analytic argument with the given index to one of this object's
 * options with the given key as a selection. If the option's value is not a
 * valid selection then an exception is thrown.
 *
 * @param index The analytic argument index whose value is set with an option
 *              with the given key.
 *
 * @param key The key used to find the option used to set the analytic argument.
 */
void Run::addSelection(int index, const QString& key)
{
   // Add the debug header.
   EDEBUG_FUNC(this,index,key);

   // Grab the option value with the given key, making sure it is a valid selection
   // value.
   QString value {_options.find(key)};
   if ( _manager->data(index,EAbstractAnalytic::Input::Role::SelectionValues).toStringList().contains(value) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Selection argument '%1' does not contain '%2' as a valid option.")
                   .arg(key)
                   .arg(value));
      throw e;
   }

   // Set the analytic argument with the given index to the valid selection value.
   _manager->set(index,value);
}

}
