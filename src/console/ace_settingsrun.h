#ifndef ACE_SETTINGSRUN_H
#define ACE_SETTINGSRUN_H
#include <QString>
#include "ace_command.h"
//



namespace Ace
{
   /*!
    * This handles the settings command for the console program of ACE. This class 
    * only requires command argument be passed to it because it does not use options 
    * at all. The majority of this class deals with parsing user input and verifying 
    * its validity along with taking the current settings of ACE and presenting them 
    * in a human readable way for the user. 
    */
   class SettingsRun
   {
   public:
      SettingsRun(const Command& command);
      void execute();
   private:
      void settings();
      QString openCLDeviceString();
      void set();
      void setOpenCL();
      void setThreads();
      void setBuffer();
      void setChunkDir();
      void setChunkPre();
      void setChunkExt();
      void setLogging();
      void list();
      void listOpenCL();
      /*!
       * The command arguments parsed out of the command line arguments of the main 
       * function, excluding the very first argument used to get to this settings class. 
       */
      Command _command;
   };
}



#endif
