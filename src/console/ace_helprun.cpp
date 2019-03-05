#include "ace_helprun.h"
#include <QTextStream>



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
   if ( _command.size() > 0 )
   {
      enum {Run,Chunkrun,Merge,Dump,Inject,Settings};
      const QStringList commands {"run","chunkrun","merge","dump","inject","settings"};
      QString command {_command.pop()};
      switch (commands.indexOf(command))
      {
      case Run:
         runHelp();
         break;
      case Chunkrun:
         runHelp();
         break;
      case Merge:
         runHelp();
         break;
      case Dump:
         runHelp();
         break;
      case Inject:
         runHelp();
         break;
      case Settings:
         runHelp();
         break;
      default:
         basic();
         break;
      }
   }
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
   QTextStream stream(stdout);
   stream << "Usage: " << _runName << " help <command>\n";
   stream << "Get help about the specific command <command>.\n\n";
   stream << "Valid commands:\n\n";
   stream << "     run: Run an analytic in normal mode. MPI is automatically detected. If no\n";
   stream << "          MPI is detected then the analytic will run in single mode.\n\n";
   stream << "chunkrun: Run an analytic in chunk run mode. This mode will execute the analytic\n\n";
   stream << "          on a subset of the data and save its results in a temporary file.\n\n";
   stream << "   merge: Merge all temporary result files from chunkruns into the finished\n";
   stream << "          output data objects of the analytic run.\n\n";
   stream << "    dump: Dump the system or user metadata of a data object to standard output\n";
   stream << "          as JSON formatted text.";
   stream << "  inject: Inject new user metadata into a data object from a JSON formatted text\n";
   stream << "          file, overwriting any user metadata the data object currently\n";
   stream << "          contains.\n\n";
   stream << "settings: Access the global persistent settings for this program to either view\n";
   stream << "          those settings or change them.\n\n";
}






/*!
 * Displays the help text for the run command.
 */
void HelpRun::runHelp()
{
}






/*!
 * Displays the help text for the chunk run command.
 */
void HelpRun::chunkrunHelp()
{
}






/*!
 * Displays the help text for the merge command.
 */
void HelpRun::mergeHelp()
{
}






/*!
 * Displays the help text for the dump command.
 */
void HelpRun::dumpHelp()
{
}






/*!
 * Displays the help text for the inject command.
 */
void HelpRun::injectHelp()
{
}






/*!
 * Displays the help text for a specific analytic type if one is specified as a
 * sub argument. If there is no sub argument this simply lists all available
 * analytic commands.
 */
void HelpRun::analyticHelp()
{
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
