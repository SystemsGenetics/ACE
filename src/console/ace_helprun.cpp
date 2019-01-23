#include "ace_helprun.h"



using namespace Ace;
//






/*!
 * Constructs a new help run object to handling a help command. 
 *
 * @param command List of command line arguments for this help run. 
 */
HelpRun::HelpRun(const Command& command):
   _command(command)
{}






/*!
 * Executes the help run command, using this classes given command line arguments 
 * for processing. 
 */
void HelpRun::execute()
{}






/*!
 * Displays the help text for the run command. 
 */
void HelpRun::runHelp()
{}






/*!
 * Displays the help text for the chunk run command. 
 */
void HelpRun::chunkrunHelp()
{}






/*!
 * Displays the help text for the merge command. 
 */
void HelpRun::mergeHelp()
{}






/*!
 * Displays the help text for the dump command. 
 */
void HelpRun::dumpHelp()
{}






/*!
 * Displays the help text for the inject command. 
 */
void HelpRun::injectHelp()
{}






/*!
 * Displays the help text for a specific analytic type if one is specified as a sub 
 * argument. If there is no sub argument this simply lists all available analytic 
 * commands. 
 */
void HelpRun::analyticHelp()
{}






/*!
 * Displays the help text for the settings command. This also describes the 
 * possible sub commands for this command and how to access their help. 
 */
void HelpRun::settingsHelp()
{}






/*!
 * Displays the help text for the settings set command. 
 */
void HelpRun::settingsSetHelp()
{}






/*!
 * Displays the help text for the settings list command. 
 */
void HelpRun::settingsListHelp()
{}
