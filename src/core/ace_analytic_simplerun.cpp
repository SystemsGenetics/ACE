#include "ace_analytic_simplerun.h"
#include "ace_analytic_abstractinput.h"
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
SimpleRun::SimpleRun(AbstractInput* base, QObject* parent):
   AbstractRun(parent),
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
