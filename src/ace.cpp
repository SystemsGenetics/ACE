#include <iostream>
#include "console.h"
#include "linuxterm.h"
namespace AccelCompEng
{



int run(const char* header, int argc, char* argv[])
{
   try
   {
      DataMap dataMap;
      LinuxTerm::stty_raw();
      LinuxTerm terminal;
      Console console(argc,argv,terminal,dataMap,header);
      console.run();
   }
   catch(Exception e)
   {
      std::cout << "Fatal Exception Caught!\n";
      std::cout << e.domain() << "::" << e.what() << "(" << e.line() <<  "): " << e.detail()
                << "\n";
      LinuxTerm::stty_cooked();
      return -1;
   }
   catch (...)
   {
      LinuxTerm::stty_cooked();
      return -1;
   }
   LinuxTerm::stty_cooked();
   return 0;
}



}
