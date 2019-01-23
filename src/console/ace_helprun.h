#ifndef ACE_HELPRUN_H
#define ACE_HELPRUN_H
#include "ace_command.h"
//



namespace Ace
{
   /*!
    * This handles the help command for the console program of ACE. This class only 
    * requires command arguments to be passed to it. This parses a assumed help 
    * command and outputs the appropriate help text based off the given command 
    * arguments. If there are no command arguments the root help text is displayed. 
    */
   class HelpRun
   {
   public:
      HelpRun(const Command& command);
      void execute();
   private:
      void runHelp();
      void chunkrunHelp();
      void mergeHelp();
      void dumpHelp();
      void injectHelp();
      void analyticHelp();
      void settingsHelp();
      void settingsSetHelp();
      void settingsListHelp();
      /*!
       * The command arguments parsed out of the command line arguments of the main 
       * function, excluding the very first argument used to get to this help class. 
       */
      Command _command;
   };
}



#endif
