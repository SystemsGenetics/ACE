#include "ace_analytic_simplerun.h"
#include "ace_analytic_abstractinput.h"
#include "eabstractanalytic_block.h"



using namespace std;
using namespace Ace::Analytic;
//






/*!
 * Constructs a new simple run object with the given abstract input object for 
 * input and an optional parent. 
 *
 * @param base Abstract input base object used to save the results this simple run 
 *             object produces. 
 *
 * @param parent Optional parent for this new simple run object. 
 */
SimpleRun::SimpleRun(AbstractInput* base, QObject* parent):
   AbstractRun(parent),
   _base(base)
{}






/*!
 * Implements the interface that is called to add a work block to be processed by 
 * this abstract run. Because this is a simple run the given work block is ignored. 
 *
 * @param block Unused work block that is ignored. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Save the result with a null pointer because this is simple. If the input is 
 *    finished then emit the finished signal. 
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
