#include "ace_helprun.h"
#include <QTextStream>
#include <QRegularExpression>
#include "../core/eabstractanalyticfactory.h"
#include "../core/eabstractanalytic_input.h"



namespace Ace
{






/*!
 * Constructs a new help run object to handle a help command with the given
 * command and run name.
 *
 * @param command List of command line arguments for this help run.
 *
 * @param runName The command name used to execute this application.
 */
HelpRun::HelpRun(const Command& command, const QString& runName)
   :
   _command(command),
   _runName(runName)
{
}






/*!
 * Executes the help run command, using this classes given command line
 * arguments for processing.
 */
void HelpRun::execute()
{
   // Check to see if there is at least one help command to process.
   if ( _command.size() > 0 )
   {
      // Create and initialize a string list and enumeration used to figure out which
      // command was given.
      enum {Run,Chunkrun,Merge,Dump,Inject,Settings};
      const QStringList commands {"run","chunkrun","merge","dump","inject","settings"};

      // Determine which help command is given by the user.
      QString command {_command.pop()};
      switch (commands.indexOf(command))
      {
      case Run:
         runHelp();
         break;
      case Chunkrun:
         chunkrunHelp();
         break;
      case Merge:
         mergeHelp();
         break;
      case Dump:
         dumpHelp();
         break;
      case Inject:
         injectHelp();
         break;
      case Settings:
         settingsHelp();
         break;
      default:
         // If the help command is not recognized then print out the basic help text.
         basic();
         break;
      }
   }

   // Else no command has been given so print out the basic help text.
   else
   {
      basic();
   }
}






/*!
 * Lists all properties of a given analytic input index to the given text
 * stream.
 *
 * @param stream The text stream which is used to output the given input
 *               properties.
 *
 * @param input The analytic input interface used to interrogate properties
 *              about a given argument index.
 *
 * @param index The specific argument index of the analytic input to interrogate
 *              properties from.
 */
void HelpRun::listAnalyticInputProperties(QTextStream& stream, const EAbstractAnalytic::Input& input, int index)
{
   // Use the type and role enumerations from the input interface.
   using Type = EAbstractAnalytic::Input::Type;
   using Role = EAbstractAnalytic::Input::Role;

   // Determine which argument type the given input argument index is and print out
   // its respective properties.
   switch (input.type(index))
   {
   case Type::Boolean:
      stream << "Boolean\n";
      break;
   case Type::Integer:
      stream << "Integer\n";

      // Because this is numeric print out its minimum and maximum accepted values.
      stream << "Minimum Value: " << input.data(index,Role::Minimum).toInt() << "\n";
      stream << "Maximum Value: " << input.data(index,Role::Maximum).toInt() << "\n";
      break;
   case Type::Double:
      stream << "Floating Point\n";

      // Because this is numeric print out its minimum and maximum accepted values.
      stream << "Minimum Value: " << input.data(index,Role::Minimum).toDouble() << "\n";
      stream << "Maximum Value: " << input.data(index,Role::Maximum).toDouble() << "\n";
      break;
   case Type::String:
      stream << "String\n";
      break;
   case Type::Selection:
      {
         stream << "Selection\n";

         // Print out the header title for legal values of this selection.
         stream << "Legal Values:";

         // Initialize a columns tracker and get all legal selection values.
         int cols {13};
         QStringList options {input.data(index,Role::SelectionValues).toStringList()};

         // Iterate through all legal value strings.
         for (auto option: options)
         {
            // If the next string causes the total columns to be greater than 80 then reset to
            // the new line along with resetting the columns tracker.
            if ( (cols + 1 + option.size()) >= 80 )
            {
               stream << "\n          ";
               cols = 14 + option.size();
            }

            // Else the columns is not beyond 80 yet so add the next string size and space.
            else
            {
               cols += 1 + option.size();
            }

            // Print out the next legal value.
            stream << " " << option;
         }

         // Print out the final new line ending the legal values selection property.
         stream << "\n";
         break;
      }
   case Type::FileIn:
      stream << "Input File\n";
      break;
   case Type::FileOut:
      stream << "Output File\n";
      break;
   case Type::DataIn:
      stream << "Input Data Object\n";
      break;
   case Type::DataOut:
      stream << "Output Data Object\n";
      break;
   }

   // If this input argument has a default value then print it to the text stream.
   QString defValue {input.data(index,Role::Default).toString()};
   if ( !defValue.isEmpty() )
   {
      stream << "Default Value: " << defValue << "\n";
   }
}






/*!
 * Outputs the what's this description of a given analytic input index to the
 * given text stream.
 *
 * @param stream The text stream which is used to output the given input
 *               description.
 *
 * @param input The analytic input interface used to interrogate the whats this
 *              description of a given argument index.
 *
 * @param index The specific argument index of the analytic input to interrogate
 *              from.
 */
void HelpRun::printDescription(QTextStream& stream, const EAbstractAnalytic::Input& input, int index)
{
   // Use the role enumeration of the analytic input class.
   using Role = EAbstractAnalytic::Input::Role;

   // Get the what's this description of the input argument with the given index,
   // splitting it up into its component words.
   QStringList words
   {
      input.data(index,Role::WhatsThis).toString().split(QRegularExpression("\\s+"))
   };

   // Continue until the list of words to print out is empty.
   while ( !words.isEmpty() )
   {
      // Create and initialize the columns tracker to the first word in the list and
      // then take that first word and output it to the given stream.
      int cols {words.first().size()};
      stream << words.takeFirst();

      // Continue while the total columns used plus the next word to be printed is less
      // than or equal to the common terminal width of 80.
      while ( !words.isEmpty() && (cols + 1 + words.first().size()) <= 80 )
      {
         // Add the size of the next word to be printed along with a required space to the
         // columns tracker and then take the word and print it.
         cols += 1 + words.first().size();
         stream << " " << words.takeFirst();
      }

      // Print the closing new line character for this line of output.
      stream << "\n";
   }
}






/*!
 * Prints out the basic root help text informing the user how to use the help
 * command system.
 */
void HelpRun::basic()
{
   // Create a text stream to standard output and print out the basic help text for
   // navigating the rest of the help system.
   QTextStream stream(stdout);
   stream << "Help: " << _runName << " help <command>\n\n"
          << "Get help about the specific command <command>.\n\n"
          << "Valid commands:\n\n"
          << "     run: Run an analytic in normal mode. MPI is automatically detected. If no\n"
          << "          MPI is detected then the analytic will run in single mode.\n\n"
          << "chunkrun: Run an analytic in chunk run mode. This mode will execute the\n"
          << "          analytic on a subset of the data and save its results in a temporary\n"
          << "          file.\n\n"
          << "   merge: Merge all temporary result files from chunkruns into the finished\n"
          << "          output data objects of the analytic run.\n\n"
          << "    dump: Dump the system or user metadata of a data object to standard output\n"
          << "          as JSON formatted text."
          << "  inject: Inject new user metadata into a data object from a JSON formatted text\n"
          << "          file, overwriting any user metadata the data object currently\n"
          << "          contains.\n\n"
          << "settings: Access the global persistent settings for this program to either view\n"
          << "          those settings or change them.\n\n";
}






/*!
 * Displays the help text for the run command.
 */
void HelpRun::runHelp()
{
   // If there is still a command to be processed then run the analytic help command.
   if ( _command.size() > 0 )
   {
      analyticHelp();
   }

   // Else this is the run help command.
   else
   {
      // Create a text stream to standard output and print out the run help text header.
      QTextStream stream(stdout);
      stream << "Command: " << _runName << " run <analytic> <options...>\n"
             << "Runs the given analytic with the given options.\n\n"
             << "Help: " << _runName << " help run <analytic>\n"
             << "Get help about running a specific analytic <analytic>.\n\n"
             << "Valid analytics:\n";

      // Iterate through all possible analytics, printing out their command line name
      // and then full name.
      EAbstractAnalyticFactory& factory {EAbstractAnalyticFactory::instance()};
      for (quint16 i = 0; i < factory.size() ;++i)
      {
         stream << factory.commandName(i) << ": " << factory.name(i) << "\n";
      }

      // Print out the final closing empty new line of the run help text.
      stream << "\n";
   }
}






/*!
 * Displays the help text for the chunk run command.
 */
void HelpRun::chunkrunHelp()
{
   // If there is still a command to be processed then run the analytic help command.
   if ( _command.size() > 0 )
   {
      analyticHelp();
   }

   // Else this is the run help command.
   else
   {
      // Create a text stream to standard output and print out the chunk run help text
      // header.
      QTextStream stream(stdout);
      stream << "Command: " << _runName << " chunkrun <index> <size> <analytic> <options...>\n"
             << "Does a chunk run of the given analytic and options, executing only a segment of\n"
             << "the total analytic and saving its temporary results to a chunk file. All these\n"
             << "chunk files are later merged with the merge command. WARNING: ALL analytic\n"
             << "options must be identical for all individual chunk run executions or they will\n"
             << "not properly merge together.\n\n"
             << "index: The specific chunk index of work that will be run on the given analytic\n"
             << "       and results saved.\n"
             << " size: The total number of chunks that the total analytic work is separated\n"
             << "       into.\n\n"
             << "Help: " << _runName << " help chunkrun <analytic>\n"
             << "Get help about chunk running a specific analytic <analytic>.\n\n"
             << "Valid analytics:\n";

      // Iterate through all possible analytics, printing out their command line name
      // and then full name.
      EAbstractAnalyticFactory& factory {EAbstractAnalyticFactory::instance()};
      for (quint16 i = 0; i < factory.size() ;++i)
      {
         stream << factory.commandName(i) << ": " << factory.name(i) << "\n";
      }

      // Print out the final closing empty new line of the chunk run help text.
      stream << "\n";
   }
}






/*!
 * Displays the help text for the merge command.
 */
void HelpRun::mergeHelp()
{
   // If there is still a command to be processed then run the analytic help command.
   if ( _command.size() > 0 )
   {
      analyticHelp();
   }

   // Else this is the run help command.
   else
   {
      // Create a text stream to standard output and print out the merge help text
      // header.
      QTextStream stream(stdout);
      stream << "Command: " << _runName << " merge <size> <analytic> <options...>\n"
             << "Merges all chunkrun output files into the finalized output of the given analytic"
             << "and options run. WARNING: ALL analytic options must be identical for the merge\n"
             << "command and all individual chunk run executions or they will not properly merge\n"
             << "together. Also all chunk run executions must be complete before the merge\n"
             << "command can be executed.\n\n"
             << " size: The total number of chunks that the total analytic work is separated\n"
             << "       into.\n\n"
             << "Help: " << _runName << " help merge <analytic>\n"
             << "Get help about merge running a specific analytic <analytic>.\n\n"
             << "Valid analytics:\n";

      // Iterate through all possible analytics, printing out their command line name
      // and then full name.
      EAbstractAnalyticFactory& factory {EAbstractAnalyticFactory::instance()};
      for (quint16 i = 0; i < factory.size() ;++i)
      {
         stream << factory.commandName(i) << ": " << factory.name(i) << "\n";
      }

      // Print out the final closing empty new line of the merge help text.
      stream << "\n";
   }
}






/*!
 * Displays the help text for the dump command.
 */
void HelpRun::dumpHelp()
{
   // Create a text stream to standard output and print out the dump help text.
   QTextStream stream(stdout);
   stream << "Command: " << _runName << " dump <path> [system|user]\n"
          << "Dumps the system or user metadata of a data object to standard output as JSON.\n\n"
          << "path: The file path to the data object whose system or user metada is dumped.\n\n";
}






/*!
 * Displays the help text for the inject command.
 */
void HelpRun::injectHelp()
{
   // Create a text stream to standard output and print out the inject help text.
   QTextStream stream(stdout);
   stream << "Command: " << _runName << " inject <path> <jsonpath>\n"
          << "Injects the given JSON text file to the data object's user metadata with the\n"
          << "given path, overwriting any user metadata the data object currently contains.\n\n"
          << "path: The file path to the data object whose user metada is overwritten.\n\n"
          << "jsonpath: The file path to the JSON text file used to overwrite the user\n"
          << "          metadata of the data object.\n\n";
}






/*!
 * Displays the help text for a specific analytic type if one is specified as a
 * sub argument. If there is no sub argument this simply lists all available
 * analytic commands.
 */
void HelpRun::analyticHelp()
{
   // Create a text stream to the standard output.
   QTextStream stream(stdout);

   // Create a command names string list and populate it with all analytic command
   // names whose indexes match the analytic type integer.
   QStringList commandNames;
   EAbstractAnalyticFactory& factory {EAbstractAnalyticFactory::instance()};
   for (quint16 i = 0; i < factory.size() ;++i)
   {
      commandNames << factory.commandName(i);
   }

   // Get the analytic command name from this run's command list.
   QString name {_command.pop()};

   // Determine the analytic type from the command name using the command names list,
   // making sure the name is a known analytic type.
   int type {commandNames.indexOf(name)};
   if ( type < 0 || type > 0xffff )
   {
      stream << "Unknown analytic '" << name << "'\n\n";
   }

   // Else the analytic type was found and should now be processed.
   else
   {
      // Create an analytic of the determined type and then create an input interface
      // from it.
      std::unique_ptr<EAbstractAnalytic> analytic {factory.make(static_cast<quint16>(type))};
      EAbstractAnalytic::Input* input {analytic->makeInput()};

      // Output the basic help header information about the created analytic to standard
      // output.
      stream << "Command: " << _runName << " run|chunkrun|merge " << name << " <options...>\n"
             << "Run the given analytic in normal, chunk, or merge mode. For chunk and merge\n"
             << "modes all separate executions MUST have the same options provided to the\n"
             << "analytic.\n\n"
             << "OPTIONS\n\n";

      // Use the role enumeration of the analytic input class.
      using Role = EAbstractAnalytic::Input::Role;

      // Iterate through all arguments of the analytic input interface class.
      for (int i = 0; i < input->size() ;++i)
      {
         // Output the help header of the input argument to standard output.
         stream << "--" << input->data(i,Role::CommandLineName).toString() << " <value>\n";
         stream << "Value Type: ";

         // Output all properties of the input argument to standard output.
         listAnalyticInputProperties(stream,*input,i);

         // Output the what's this description of the input argument to standard output.
         printDescription(stream,*input,i);
         stream << "\n";
      }
   }
}






/*!
 * Displays the help text for the settings command. This also describes the
 * possible sub commands for this command and how to access their help.
 */
void HelpRun::settingsHelp()
{
   // Define a function to print out the basic help text of the settings command.
   auto f
   {
      [this]()
      {
         // Create a text stream to standard output and then print the basic help text for
         // the settings command.
         QTextStream stream(stdout);
         stream << "Command: " << _runName << " settings [set|list|] ...\n"
                << "Executes the settings command that accesses the persistent settings of this\n"
                << "application. These settings are persistent across all instances of this\n"
                << "application in both the console and GUI version. The basic command with no sub\n"
                << "arguments will simply print out the current value of all settings. The set sub\n"
                << "command will set a specific setting to a new value. The list sub command will\n"
                << "list valid values for certain settings.\n\n"
                << "Help: " << _runName << " help settings [set|list]\n"
                << "Get help about setting sub commands set or list.\n\n";
      }
   };

   // Check to see if there is a command to be processed.
   if ( _command.size() > 0 )
   {
      // Initialize an enumeration and string list used for determining the setting help
      // command given.
      enum {Set,List};
      const QStringList commands {"set","list"};

      // Determine the settings help command given and call the appropriate method,
      // printing the basic settings help text if the command is not recognized.
      QString command {_command.pop()};
      switch (commands.indexOf(command))
      {
      case Set:
         settingsSetHelp();
         break;
      case List:
         settingsListHelp();
         break;
      default:
         f();
         break;
      }
   }

   // Else there is no command to process to print out the basic settings help text
   // information.
   else
   {
      f();
   }
}






/*!
 * Displays the help text for the settings set command. If an additional command
 * argument is given and it is a valid setting to set its help text is displayed
 * instead.
 */
void HelpRun::settingsSetHelp()
{
   // Initialize an enumeration and string list used to determine what setting set
   // command was given to get help on, if any.
   enum {CUDA,OpenCL,Threads,Buffer,ChunkDir,ChunkPre,ChunkExt,Logging};
   QStringList list {"cuda","opencl","threads","buffer","chunkdir","chunkpre","chunkext","logging"};

   // Create an empty command string, setting it to this run's next command argument
   // is any exists.
   QString command;
   if ( _command.size() > 0 )
   {
      command = _command.pop();
   }

   // Determine which setting set command was provided to get help text about.
   switch (list.indexOf(command))
   {
   case CUDA:
      settingsSetCUDAHelp();
      break;
   case OpenCL:
      settingsSetOpenCLHelp();
      break;
   case Threads:
      settingsSetThreadsHelp();
      break;
   case Buffer:
      settingsSetBufferHelp();
      break;
   case ChunkDir:
      settingsSetChunkDirHelp();
      break;
   case ChunkPre:
      settingsSetChunkPreHelp();
      break;
   case ChunkExt:
      settingsSetChunkExtHelp();
      break;
   case Logging:
      settingsSetLoggingHelp();
      break;
   default:
      {
         // If the setting set command was not recognized or was empty then print the basic
         // help text for the setting set command.
         QTextStream stream(stdout);
         stream << "Command: " << _runName << " settings set <key> <value>\n"
                << "Updates a persistent setting with the given key to the new given value.\n\n"
                << "  key: The key of the setting that will be updated to a new value. Valid keys\n"
                << "       are cuda, opencl, threads, buffer, chunkdir, chunkpre, chunkext, and\n"
                << "       logging.\n\n"
                << "value: The new value of the given setting.\n\n"
                << "Help: " << _runName << " help settings set <key>\n"
                << "Get help about a specific setting to set with the given key.\n\n";
         break;
      }
   }
}






/*!
 * Displays the help text for the settings set cuda command.
 */
void HelpRun::settingsSetCUDAHelp()
{
   // Create a text stream to standard output and print the settings set cuda command
   // help text.
   QTextStream stream(stdout);
   stream << "Command: " << _runName << " settings set cuda <device>\n"
          << "Updates the CUDA device setting.\n\n"
          << "device: The integer indexed CUDA device that this application will use for\n"
          << "        high performance computing.\n\n";
}






/*!
 * Displays the help text for the settings set opencl command.
 */
void HelpRun::settingsSetOpenCLHelp()
{
   // Create a text stream to standard output and print the settings set opencl
   // command help text.
   QTextStream stream(stdout);
   stream << "Command: " << _runName << " settings set opencl <device>\n"
          << "Updates the OpenCL device setting.\n\n"
          << "device: The integer indexed OpenCL platform and device that this application\n"
             "        will use for high performance computing. The platform index integer\n"
             "        must be first, separated by a colon, and then the device index integer.\n\n";
}






/*!
 * Displays the help text for the settings set threads command.
 */
void HelpRun::settingsSetThreadsHelp()
{
   // Create a text stream to standard output and print the settings set threads
   // command help text.
   QTextStream stream(stdout);
   stream << "Command: " << _runName << " settings set threads <number>\n"
          << "Updates the thread size setting. The thread size is the number of threads used\n"
          << "for each CUDA or OpenCL device used in an analytic run. Multiple threads are\n"
          << "used to make sure the high performance device is fully utilized.\n\n"
          << "number: The number of threads used for each CUDA or OpenCL device.\n\n";
}






/*!
 * Displays the help text for the settings set buffer command.
 */
void HelpRun::settingsSetBufferHelp()
{
   // Create a text stream to standard output and print the settings set buffer
   // command help text.
   QTextStream stream(stdout);
   stream << "Command: " << _runName << " settings set buffer <size>\n"
          << "Updates the buffer size setting. The buffer size is the number of extra work\n"
          << "blocks each slave node in an mpi run is given in addition to the work block it\n"
          << "is currently working on. This is used to prevent a slave node from wasting time\n"
          << "waiting for another work block instead of working on one. If the buffer size is\n"
          << "too high the MPI run could take longer if the speed of work block execution\n"
          << "differs between slave nodes.\n\n"
          << "size: The new buffer size or number of additional works blocks for each\n"
          << "      slave node in an MPI run.\n\n";
}






/*!
 * Displays the help text for the settings set chunkdir command.
 */
void HelpRun::settingsSetChunkDirHelp()
{
   // Create a text stream to standard output and print the settings set chunkdir
   // command help text.
   QTextStream stream(stdout);
   stream << "Command: " << _runName << " settings set chunkdir <directory>\n"
          << "Updates the directory where all temporary chunk files are stored when running\n"
          << "an analytic in chunk mode and used to find the created chunk files when merging\n"
          << "them together into the analytic's output.\n\n"
          << "directory: The absolute path to an existing directory used to store and\n"
          << "           retrieve temporary chunk files. The user running the chunk run and\n"
          << "           merge commands must have write access to the directory.\n\n";
}






/*!
 * Displays the help text for the settings set chunkpre command.
 */
void HelpRun::settingsSetChunkPreHelp()
{
   // Create a text stream to standard output and print the settings set chunkpre
   // command help text.
   QTextStream stream(stdout);
   stream << "Command: " << _runName << " settings set chunkpre <name>\n"
          << "Updates the name prepended and used for each temporary chunk file. The is\n"
          << "prepended before the number for each temporary chunk file.\n\n"
          << "name: String prepended before the number of each temporary chunk file.\n\n";
}






/*!
 * Displays the help text for the settings set chunkext command.
 */
void HelpRun::settingsSetChunkExtHelp()
{
   // Create a text stream to standard output and print the settings set chunkext
   // command help text.
   QTextStream stream(stdout);
   stream << "Command: " << _runName << " settings set chunkext <ext>\n"
          << "Updates the extension used for each temporary chunk file.\n\n"
          << "ext: The extension used for each temporary chunk file.\n\n";
}






/*!
 * Displays the help text for the settings set logging command.
 */
void HelpRun::settingsSetLoggingHelp()
{
   // Create a text stream to standard output and print the settings set logging
   // command help text.
   QTextStream stream(stdout);
   stream << "Command: " << _runName << " settings set logging <on/off>\n"
          << "Acticates or disables the built in ACE logging system for this application. The\n"
          << "logging system can be accessed using the utility command acelog. If logging is\n"
          << "on then running an analytic will not start until a logging client has connected\n"
          << "to the application.\n\n";
}






/*!
 * Displays the help text for the settings list command.
 */
void HelpRun::settingsListHelp()
{
   // Create a text stream to standard output and print the settings list command
   // help text.
   QTextStream stream(stdout);
   stream << "Command: " << _runName << " settings list <cuda/opencl>\n"
          << "Lists all available OpenCL or CUDA devices on the current platform.\n\n";
}

}
