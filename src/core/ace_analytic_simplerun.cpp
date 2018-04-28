#include "ace_analytic_simplerun.h"
#include "ace_analytic_iobase.h"
#include "eabstractanalytic_block.h"



using namespace std;
using namespace Ace::Analytic;
//






/*!
 *
 * @param base  
 *
 * @param parent  
 */
SimpleRun::SimpleRun(IOBase* base, QObject* parent):
   Run(parent),
   _base(base)
{}






/*!
 *
 * @param block  
 */
void SimpleRun::addWork(std::unique_ptr<EAbstractAnalytic::Block>&& block)
{
   Q_UNUSED(block)
   _base->saveResult(nullptr);
   if ( _base->isFinished() )
   {
      emit finished();
   }
}
