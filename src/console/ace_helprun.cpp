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
   if ( _command.size() > 0 )
   {
      enum {Set,List};
      const QStringList commands {"set","list"};
      QString command {_command.pop()};
      switch (commands.indexOf(command))
      {
      case Set:
         settingsSetHelp();
         break;
      case List:
         settingsListHelp();
         break;
      }
   }
   else
   {
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
}






/*!
 * Displays the help text for the settings set command.
 */
void HelpRun::settingsSetHelp()
{
}






/*!
 * Displays the help text for the settings list command.
 */
void HelpRun::settingsListHelp()
{
}

}
