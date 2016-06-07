#include <iostream>
#include "console.h"
#include "linuxterm.h"
namespace AccelCompEng
{



int run(int argc, char* argv[])
{
   try
   {
      DataMap dataMap;
      LinuxTerm::stty_raw();
      LinuxTerm terminal;
      Console console(argc,argv,terminal,dataMap);
      console.run();
   }
   catch (...)
   {
      LinuxTerm::stty_cooked();
      throw;
   }
   LinuxTerm::stty_cooked();
   return 0;
}



}
