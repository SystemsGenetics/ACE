#ifndef ACE_SETTINGSRUN_H
#define ACE_SETTINGSRUN_H
#include <QString>
#include "ace_command.h"



/*!
 * This contains all private classes used internally by the ACE library and
 * should never be accessed by a developer using this library.
 */
namespace Ace
{
   /*!
    * This handles the settings command for the console program of ACE. This class
    * only requires command argument be passed to it because it does not use
    * options at all. The majority of this class deals with parsing user input and
    * verifying its validity along with taking the current settings of ACE and
    * presenting them in a human readable way for the user.
    */
   class SettingsRun
   {
   public:
      SettingsRun(const Command& command);
      void execute();
   private:
      void settings();
      QString cudaDeviceString();
      QString openCLDeviceString();
      void set();
      void setCUDA();
      void setOpenCL();
      void setThreads();
      void setBuffer();
      void setChunkDir();
      void setChunkPre();
      void setChunkExt();
      void setLogging();
      void list();
      void listCUDA();
      void listOpenCL();
      /*!
       * The command arguments parsed out of the command line arguments of the main
       * function, excluding the very first argument used to get to this settings
       * class.
       */
      Command _command;
   };
}

#endif
