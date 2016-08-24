#include "analytic.h"
namespace AccelCompEng
{



void Analytic::execute(GetOpts& ops, Terminal& tm)
{
   if (CLContext::is_initd())
   {
      execute_cl(ops,tm);
   }
   else
   {
      execute_pn(ops,tm);
   }
}



}
