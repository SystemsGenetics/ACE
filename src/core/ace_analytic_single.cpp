#include "ace_analytic_single.h"
#include <QTimer>
#include "ace_analytic_simplerun.h"
#include "ace_analytic_serialrun.h"
#include "ace_analytic_openclrun.h"
#include "ace_analytic_cudarun.h"
#include "ace_settings.h"
#include "edebug.h"
#include "eabstractanalyticblock.h"



namespace Ace
{
namespace Analytic
{






/*!
 * Implements the interface that tests if this abstract input is finished and
 * received all result blocks for its analytic.
 *
 * @return True if this abstract input is finished or false otherwise.
 */
bool Single::isFinished() const
{
   EDEBUG_FUNC(this);
   return _nextResult >= analytic()->size();
}






/*!
 * Constructs a new single run manager with the given analytic type.
 *
 * @param type Analytic type to use for this analytic run.
 */
Single::Single(quint16 type)
   :
   AbstractManager(type)
{
   EDEBUG_FUNC(this,type);
}






/*!
 * Implements the interface that returns the next expected result block index to
 * maintain order of result blocks.
 *
 * @return The next expected result block index to maintain order.
 */
int Single::index() const
{
   EDEBUG_FUNC(this);
   return _nextResult;
}






/*!
 * Implements the interface that is called to save the given result block to the
 * underlying analytic and it can be assumed that the index order is maintained
 * from least to greatest.
 *
 * @param result The result block that is saved to the underlying analytic.
 */
void Single::writeResult(std::unique_ptr<EAbstractAnalyticBlock>&& result)
{
   // Add the debug header.
   EDEBUG_FUNC(this,result.get());

   // Write the given result block to the underlying analytic. If this manager is
   // finished then emit the done signal.
   AbstractManager::writeResult(std::move(result),_nextResult++);
   if ( isFinished() )
   {
      emit done();
   }
}






/*!
 * Implements the interface that is called once to begin the analytic run for
 * this manager after all argument input has been set. This implementation
 * starts this object's process slot.
 */
void Single::start()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   analytic()->initializeOutputs();

   // Setup OpenCL, setup serial if OpenCL fails, and then connect this object's
   // abstract runner class finished signal with this manager's finish slot.
   setupCUDA();
   setupOpenCL();
   setupSerial();
   connect(_runner,&AbstractRun::finished,this,&AbstractManager::finish);

   // Initialize analytic processing by calling this object's process slot.
   QTimer::singleShot(0,this,&Single::process);
}






/*!
 * Called to add all work, blocks or none if in simple mode, to this manager's
 * abstract run object.
 */
void Single::process()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // While the next work index is less than the size of this object's analytic do
   // the following steps.
   while ( _nextWork < analytic()->size() )
   {
      // If this object has a simple run object then add work with a null pointer, else
      // add work with the next work block generated from this object's analytic.
      if ( _simple )
      {
         _runner->addWork(nullptr);
      }
      else
      {
         _runner->addWork(makeWork(_nextWork));
      }

      // Increment the next work index by one.
      ++_nextWork;
   }
}






/*!
 * Attempts to initialize a CUDA run object for block processing for this
 * manager. If successful sets this manager's abstract run pointer.
 */
void Single::setupCUDA()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // If the singleton settings object has a valid CUDA device pointer and this
   // manager's analytic creates a valid abstract CUDA object then create a new CUDA
   // run object, setting this manager's run pointer to the new object.
   Settings& settings {Settings::instance()};
   if ( settings.cudaDevicePointer() )
   {
      EAbstractAnalyticCUDA* cuda {analytic()->makeCUDA()};
      if ( cuda )
      {
         _runner = new CUDARun(cuda,settings.cudaDevicePointer(),this,this);
      }
   }
}






/*!
 * Attempts to initialize an OpenCL run object for block processing for this
 * manager. If successful sets this manager's abstract run pointer.
 */
void Single::setupOpenCL()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // If the singleton settings object has a valid OpenCL device pointer and this
   // manager's analytic creates a valid abstract OpenCL object then create a new
   // OpenCL run object, setting this manager's run pointer to the new object.
   if ( _runner )
   {
      return;
   }

   // .
   Settings& settings {Settings::instance()};
   if ( settings.openCLDevicePointer() )
   {
      EAbstractAnalyticOpenCL* opencl {analytic()->makeOpenCL()};
      if ( opencl )
      {
         _runner = new OpenCLRun(opencl,settings.openCLDevicePointer(),this,this);
      }
   }
}






/*!
 * Initializes this object's abstract run object as a serial run or simple run
 * if serial is not available. This does nothing if this object's abstract run
 * object has already been created.
 */
void Single::setupSerial()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // If this object's abstract run object has already been set then do nothing and
   // exit, else go to the next step.
   if ( _runner )
   {
      return;
   }

   // If this manager's analytic creates a valid abstract serial object then create a
   // new serial run object and set it to this manager's abstract run object, else
   // create a new simple run object and set it to this manager's abstract run
   // object.
   if ( EAbstractAnalyticSerial* serial = analytic()->makeSerial() )
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
}
