#include "unit.h"
#include "ace.h"



const char* unit::headerStr {nullptr};
int unit::numTestsDone {0};



AnalyticPlugin* AccelCompEng::new_analytic(const std::string& type)
{
   AnalyticPlugin* ret = nullptr;
   return ret;
}



int main(int argc, char* argv[])
{
   unit::initiate();
   if (unit::getopts::main()&&
       unit::filemem::main()&&
       unit::fstring::main()&&
       unit::histitem::main()&&
       unit::history::main()&&
       unit::kincfile::main()&&
       unit::datamap::main())
   {
      unit::complete();
   }
   else
   {
      exit(1);
   }
   return AccelCompEng::run(argc,argv);
}
