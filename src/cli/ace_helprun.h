#ifndef ACE_HELPRUN_H
#define ACE_HELPRUN_H
#include "ace_command.h"
#include "../core/global.h"
class QTextStream;



/*!
 * This contains all private classes used internally by the ACE library and
 * should never be accessed by a developer using this library.
 */
namespace Ace
{
   /*!
    * This handles the help command for the CLI program of ACE. This class only
    * requires command arguments to be passed to it. This parses a assumed help
    * command and outputs the appropriate help text based off the given command
    * arguments. If there are no command arguments the root help text is displayed.
    */
   class HelpRun
   {
   public:
      HelpRun(const Command& command, const QString& runName);
      void execute();
   private:
      static void listAnalyticInputProperties(QTextStream& stream, const EAbstractAnalyticInput& input, int index);
      static void printDescription(QTextStream& stream, const EAbstractAnalyticInput& input, int index);
   private:
      void basic();
      void runHelp();
      void chunkrunHelp();
      void mergeHelp();
      void dumpHelp();
      void injectHelp();
      void analyticHelp();
      void settingsHelp();
      void settingsSetHelp();
      void settingsSetCUDAHelp();
      void settingsSetOpenCLHelp();
      void settingsSetThreadsHelp();
      void settingsSetBufferHelp();
      void settingsSetChunkDirHelp();
      void settingsSetChunkPreHelp();
      void settingsSetChunkExtHelp();
      void settingsSetLoggingHelp();
      void settingsSetLogPortHelp();
      void settingsListHelp();
      /*!
       * The command arguments parsed out of the command line arguments of the main
       * function, excluding the very first argument used to get to this help class.
       */
      Command _command;
      /*!
       * The command name used to execute this application.
       */
      QString _runName;
   };
}

#endif
