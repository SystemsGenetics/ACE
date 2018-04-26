#include "ace_analytic_single.h"
#include <QTimer>
#include "ace_analytic_simplerun.h"
#include "ace_analytic_serialrun.h"
#include "ace_analytic_openclrun.h"
#include "eabstractanalytic_block.h"
#include "eabstractanalytic_serial.h"
#include "eabstractanalytic_opencl.h"
#include "ace_settings.h"



using namespace std;
using namespace Ace::Analytic;
//






/*!
 * Constructs a new single run session manager with the given analytic type. 
 *
 * @param type Analytic type to use for this analytic run. 
 */
Single::Single(quint16 type):
   Manager(type)
{
   Settings& settings {Settings::instance()};
   EAbstractAnalytic::OpenCL* opencl {nullptr};
   if ( settings.device() )
   {
      opencl = analytic()->makeOpenCL();
      if ( opencl )
      {
         _runner = new OpenCLRun(opencl,settings.device(),this,this);
      }
   }
   if ( !opencl )
   {
      if ( EAbstractAnalytic::Serial* serial = analytic()->makeSerial() )
      {
         _runner = new SerialRun(serial,this,this);
      }
      else
      {
         _runner = new SimpleRun(this,this);
      }
   }
   connect(_runner,&Run::finished,this,&Manager::finish);
}






/*!
 */
bool Single::hasWork() const
{
   return _nextWork < analytic()->size();
}






/*!
 */
bool Single::isFinished() const
{
   return _nextResult >= analytic()->size();
}






/*!
 */
std::unique_ptr<EAbstractAnalytic::Block> Single::makeWork()
{
   if ( _nextWork >= analytic()->size() )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Cannot make work block when all indexes have been made."));
      throw e;
   }
   unique_ptr<EAbstractAnalytic::Block> ret {analytic()->makeBlock(_nextWork)};
   if ( !ret )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Analytic returned null work block in serial mode."));
      throw e;
   }
   if ( ret->index() != _nextWork )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Analytic returned work block with index %1 when it should be %2.")
                   .arg(ret->index())
                   .arg(_nextWork));
      throw e;
   }
   ++_nextWork;
   return ret;
}






/*!
 */
int Single::index() const
{
   return _nextResult;
}






/*!
 *
 * @param result  
 */
void Single::writeResult(std::unique_ptr<EAbstractAnalytic::Block>&& result)
{
   if ( result->index() != _nextResult )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Given result block with index %1 when it should be %2.")
                   .arg(result->index())
                   .arg(_nextResult));
      throw e;
   }
   analytic()->process(result.get());
   result.reset();
   ++_nextResult;
}






/*!
 * Implements the interface that is called once to begin the analytic run for this 
 * manager. This implementation initializes OpenCL if available or serial 
 * processing if not. Either way it begins execution of the analytic once its 
 * processing objects are setup. 
 */
void Single::start()
{
   _runner->start();
}
