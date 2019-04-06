#include "ace_analytic_simplerun.h"
#include "ace_analytic_abstractinput.h"
#include "eabstractanalyticblock.h"
#include "edebug.h"



using namespace std;
using namespace Ace::Analytic;
//






/*!
 * Implements the interface that is called to add a work block to be processed by 
 * this abstract run. Because this is a simple run the given work block is ignored. 
 *
 * @param block Unused work block that is ignored. 
 */
void SimpleRun::addWork(std::unique_ptr<EAbstractAnalyticBlock>&& block)
{
   EDEBUG_FUNC(this,block.get())

   // Save the result with a null pointer because this is simple. If the input is 
   // finished then emit the finished signal. 
   Q_UNUSED(block)
   _base->saveResult(nullptr);
   if ( _base->isFinished() )
   {
      emit finished();
   }
}






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
{
   EDEBUG_FUNC(this,base,parent)
}
