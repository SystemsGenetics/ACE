#include "ace_analytic_singlerun.h"
#include <QTimer>
#include "eabstractanalytic_serial.h"
#include "eabstractanalytic_block.h"



using namespace std;
using namespace Ace::Analytic;
//






/*!
 * Constructs a new single run session manager with the given analytic type. 
 *
 * @param type Analytic type to use for this analytic run. 
 */
SingleRun::SingleRun(quint16 type):
   Manager(type)
{
   _serial = analytic()->makeSerial();
}






/*!
 * Implements the interface that is called once to begin the analytic run for this 
 * manager. This implementation initializes OpenCL if available or serial 
 * processing if not. Either way it begins execution of the analytic once its 
 * processing objects are setup. 
 */
void SingleRun::start()
{
   QTimer::singleShot(0,this,&SingleRun::executeSerial);
}






/*!
 * Called repeatedly to execute and serially process another work block from this 
 * manager's analytic until all blocks have been processed. This slot calls itself 
 * at the end if there are more work blocks to be processed. 
 */
void SingleRun::executeSerial()
{
   if ( _next < analytic()->size() )
   {
      unique_ptr<EAbstractAnalytic::Block> work {analytic()->makeBlock(_next)};
      if ( work )
      {
         unique_ptr<EAbstractAnalytic::Block> result {_serial->execute(work.get())};
         analytic()->readBlock(_next,result.get());
      }
      else
      {
         analytic()->readBlock(_next,nullptr);
      }
      next();
      QTimer::singleShot(0,this,&SingleRun::executeSerial);
   }
   else
   {
      finish();
   }
}






/*!
 * Increments the next index to be processed and the percent complete, emitting a 
 * progressed signal if the percent complete changes. 
 */
void SingleRun::next()
{
   int percentComplete {100*_next++/analytic()->size()};
   if ( _percentComplete != percentComplete )
   {
      _percentComplete = percentComplete;
      emit progressed(_percentComplete);
   }
}
