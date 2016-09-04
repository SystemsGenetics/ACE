#include "unit.h"
using namespace AccelCompEng;



const char* unit::headerStr {nullptr};
int unit::numTestsDone {0};



int main(int argc, char* argv[])
{
   UTests tests;
   add_nvmemory(tests);
   add_fstring(tests);
   add_history(tests);
   if (!tests.execute())
   {
      exit(1);
   }
   AceTestFactory factory;
   return AccelCompEng::run("ACE_TEST",factory,argc,argv);
}
