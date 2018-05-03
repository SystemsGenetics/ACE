#ifndef ACE_SETTINGSRUN_H
#define ACE_SETTINGSRUN_H
#include <QString>
#include "ace_command.h"
//



namespace Ace
{
   /*!
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
      void list();
      void listOpenCL();
      /*!
       */
      Command _command;
   };
}



#endif
