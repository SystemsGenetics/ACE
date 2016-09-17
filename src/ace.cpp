#include <iostream>
#include "../ace.h"
namespace AccelCompEng
{



int run(const char* header, Factory& factory, int argc, char* argv[], void (*callBack)(Console&))
{
   try
   {
      // Initialize data manager and set terminal to raw mode, then initialize terminal, and lastly
      // initialize the main console class. If there is a callback call it, else step into console.
      DataMap dataMap(factory);
      LinuxTerm::stty_raw();
      LinuxTerm terminal;
      Console console(argc,argv,terminal,factory,dataMap,header);
      if ( callBack )
      {
         callBack(console);
      }
      else
      {
         console.run();
      }
   }
   // Catch any possible exception, report on it if it is known, and most importantly put the
   // terminal back into cooked mode.
   catch(Exception e)
   {
      std::cout << "Fatal Exception Caught!\n";
      std::cout << e.what() << "(" << e.function() << ":" << e.line() <<  ")";
      if ( e.detail() )
      {
         std::cout << ": " << e.detail();
      }
      std::cout << "\n";
      LinuxTerm::stty_cooked();
      return -1;
   }
   catch (std::exception& e)
   {
      std::cout << "Fatal STD Exception Caught!\n";
      std::cout << e.what() << "\n";
      LinuxTerm::stty_cooked();
      return -1;
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
