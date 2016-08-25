#include "unit.h"
using namespace AccelCompEng;



const char* unit::headerStr {nullptr};
int unit::numTestsDone {0};



int main(int argc, char* argv[])
{
   unit::initiate();
   if (unit::getopts::main()&&
       unit::filemem::main()&&
       unit::fstring::main()&&
       unit::histitem::main()&&
       unit::history::main()&&
       unit::file::main()&&
       unit::datamap::main())
   {
      unit::complete();
   }
   else
   {
      exit(1);
   }
   AceTestFactory factory;
   return AccelCompEng::run("ACE_TEST",factory,argc,argv);
}
