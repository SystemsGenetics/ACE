#include "ace_helprun.h"
#include <QTextStream>
#include "../core/eabstractanalyticfactory.h"
#include "../core/eabstractanalytic.h"
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
   QTextStream stream(stdout);
   QStringList commandNames;
   EAbstractAnalyticFactory& factory {EAbstractAnalyticFactory::instance()};
   for (quint16 i = 0; i < factory.size() ;++i)
   {
      commandNames << factory.commandName(i);
   }
   QString name {_command.pop()};
   int type {commandNames.indexOf(name)};
   if ( type < 0 || type > 0xffff )
   {
      stream << "Unknown analytic '" << name << "'\n\n";
   }
   else
   {
      std::unique_ptr<EAbstractAnalytic> analytic {factory.make(static_cast<quint16>(type))};
      EAbstractAnalytic::Input* input {analytic->makeInput()};
      stream << "Command: " << _runName << " run|chunkrun|merge " << name << " <options...>\n"
             << "Run the given analytic in normal, chunk, or merge mode. For chunk and merge\n"
             << "modes all separate executions MUST have the same options provided to the\n"
             << "analytic.\n\n"
             << "OPTIONS\n\n";
      using Type = EAbstractAnalytic::Input::Type;
      using Role = EAbstractAnalytic::Input::Role;
      for (int i = 0; i < input->size() ;++i)
      {
         stream << "--" << input->data(i,Role::CommandLineName).toString() << " <value>\n";
         stream << "Value Type: ";
         switch (input->type(i))
         {
         case Type::Boolean:
            stream << "Boolean\n";
            break;
         case Type::Integer:
            stream << "Integer\n";
            stream << "Minimum Value: " << input->data(i,Role::Minimum).toInt() << "\n";
            stream << "Maximum Value: " << input->data(i,Role::Maximum).toInt() << "\n";
            break;
         case Type::Double:
            stream << "Floating Point\n";
            stream << "Minimum Value: " << input->data(i,Role::Minimum).toDouble() << "\n";
            stream << "Maximum Value: " << input->data(i,Role::Maximum).toDouble() << "\n";
            break;
         case Type::String:
            stream << "String\n";
            break;
         case Type::Selection:
            {
               stream << "Selection\n";
               stream << "Legal Values:";
               int cols {13};
               QStringList options {input->data(i,Role::SelectionValues).toStringList()};
               for (auto option: options)
               {
                  if ( (cols + 1 + option.size()) >= 80 )
                  {
                     stream << "\n          ";
                     cols = 14 + option.size();
                  }
                  else
                  {
                     cols += 1 + option.size();
                  }
                  stream << " " << option;
               }
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
         QString defValue {input->data(i,Role::Default).toString()};
         if ( !defValue.isEmpty() )
         {
            stream << "Default Value: " << input->data(i,Role::Default).toString() << "\n";
         }
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
