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
   if ( settings.openCLDevicePointer() )
   {
      opencl = analytic()->makeOpenCL();
      if ( opencl )
      {
         _runner = new OpenCLRun(opencl,settings.openCLDevicePointer(),this,this);
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
         _simple = true;
      }
   }
   connect(_runner,&Run::finished,this,&Manager::finish);
}






/*!
 */
bool Single::isFinished() const
{
   return _nextResult >= analytic()->size();
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
   int percentComplete {_nextResult*100/analytic()->size()};
   if ( percentComplete != _percentComplete )
   {
      _percentComplete = percentComplete;
      emit progressed(_percentComplete);
   }
}






/*!
 * Implements the interface that is called once to begin the analytic run for this 
 * manager. This implementation initializes OpenCL if available or serial 
 * processing if not. Either way it begins execution of the analytic once its 
 * processing objects are setup. 
 */
void Single::start()
{
   QTimer::singleShot(0,this,&Single::process);
}






/*!
 */
void Single::process()
{
   while ( _nextWork < analytic()->size() )
   {
      if ( _simple )
      {
         _runner->addWork(unique_ptr<EAbstractAnalytic::Block>(new EAbstractAnalytic::Block(_nextWork)));
      }
      else
      {
         unique_ptr<EAbstractAnalytic::Block> work {analytic()->makeBlock(_nextWork)};
         if ( !work )
         {
            E_MAKE_EXCEPTION(e);
            e.setTitle(tr("Logic Error"));
            e.setDetails(tr("Analytic returned null work block in serial mode."));
            throw e;
         }
         if ( work->index() != _nextWork )
         {
            E_MAKE_EXCEPTION(e);
            e.setTitle(tr("Logic Error"));
            e.setDetails(tr("Analytic returned work block with index %1 when it should be %2.")
                         .arg(work->index())
                         .arg(_nextWork));
            throw e;
         }
         _runner->addWork(std::move(work));
      }
      ++_nextWork;
   }
}
