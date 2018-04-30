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
   AbstractManager(type)
{
   setupOpenCL();
   setupSerial();
   connect(_runner,&AbstractRun::finished,this,&AbstractManager::finish);
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
   AbstractManager::writeResult(std::move(result),_nextResult++);
   if ( isFinished() )
   {
      emit done();
   }
}






/*!
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
         _runner->addWork(nullptr);
      }
      else
      {
         _runner->addWork(makeWork(_nextWork));
      }
      ++_nextWork;
   }
}






/*!
 */
void Single::setupOpenCL()
{
   Settings& settings {Settings::instance()};
   if ( settings.openCLDevicePointer() )
   {
      EAbstractAnalytic::OpenCL* opencl {analytic()->makeOpenCL()};
      if ( opencl )
      {
         _runner = new OpenCLRun(opencl,settings.openCLDevicePointer(),this,this);
      }
   }
}






/*!
 */
void Single::setupSerial()
{
   if ( !_runner )
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
}
