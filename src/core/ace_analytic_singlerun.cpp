#include "ace_analytic_singlerun.h"
#include <QTimer>
#include "eabstractanalytic_serial.h"



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
      analytic()->readBlock(_next,_serial->execute(analytic()->makeBlock(_next)));
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
