#include "ace_analytic_iobase.h"
#include "eabstractanalytic_block.h"



using namespace std;
using namespace Ace::Analytic;
//






/*!
 */
IOBase::~IOBase()
{
   for (auto result: _hopper.values())
   {
      delete result;
   }
}






/*!
 *
 * @param result  
 *
 * @param analytic  
 *
 * @param next  
 */
void IOBase::hopperSaveResult(std::unique_ptr<EAbstractAnalytic::Block>&& result, EAbstractAnalytic* analytic, int* next)
{
   if ( result->index() == *next )
   {
      analytic->process((*next)++,result.get());
      result.reset();
      while ( _hopper.contains(*next) )
      {
         unique_ptr<EAbstractAnalytic::Block> result {_hopper.take(*next)};
         analytic->process((*next)++,result.get());
      }
   }
   else
   {
      _hopper.insert(result->index(),result.release());
   }
}
