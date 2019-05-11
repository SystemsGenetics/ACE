#include "ace_analytic_cudarunthread.h"
#include "eexception.h"
#include "edebug.h"
#include "eabstractanalyticblock.h"
#include "eabstractanalyticcudaworker.h"



namespace Ace
{
namespace Analytic
{






/*!
 * Constructs a new thread object with the given abstract CUDA worker and
 * optional parent.
 *
 * @param worker An abstract CUDA worker used to do the actual processing of
 *               work blocks into result blocks.
 *
 * @param parent Optional parent of this new thread object.
 */
CUDARunThread::CUDARunThread(std::unique_ptr<EAbstractAnalyticCUDAWorker>&& worker, QObject* parent)
   :
   QThread(parent),
   _worker(worker.release())
{
   EDEBUG_FUNC(this,worker.get(),parent);
   _worker->setParent(this);
}






/*!
 * Executes the processing of the given work block on a separate thread from the
 * one called this method. This returns immediately after starting the separate
 * thread. If this thread already contains a result block then an exception is
 * thrown.
 *
 * @param block The work block that is processed on a separate thread.
 */
void CUDARunThread::execute(std::unique_ptr<EAbstractAnalyticBlock>&& block)
{
   // Add the debug header.
   EDEBUG_FUNC(this,block.get());

   // If this thread already contains a result block then throw an exception, else go
   // to the next step.
   if ( _result )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Cannot execute CUDA engine piston that contains a result."));
      throw e;
   }

   // Delete any previous work block this object contains, set the given work block
   // as this objects new work block and then start its separate thread.
   delete _work;
   _work = block.release();
   _work->setParent(this);
   ++_switch;
}






/*!
 * Returns the result block produces on this object's separate thread after
 * finishing. If the separate thread threw an exception that exception is thrown
 * again on the thread calling this method. If there is not result block an
 * exception is also thrown.
 *
 * @return Result block produced by this object's separate thread execution.
 */
std::unique_ptr<EAbstractAnalyticBlock> CUDARunThread::result()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // If this object has a saved exception from its separate thread then copy it and
   // throw it on this thread, else go to the next step.
   if ( _exception )
   {
      EException e(*_exception);
      delete _exception;
      _exception = nullptr;
      throw e;
   }

   // If this object does not contain a result block then throw an exception, else go
   // the next step.
   if ( !_result )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Cannot get result from CUDA engine piston that contains none."));
      throw e;
   }

   // Release this object's saved result block from its ownership and return it.
   _result->setParent(nullptr);
   std::unique_ptr<EAbstractAnalyticBlock> ret {_result};
   _result = nullptr;
   return ret;
}






/*!
 * Executes this object's saved work block on its separate thread, saving the
 * result block. If any exception is thrown within this separate thread it is
 * caught and saved.
 */
void CUDARunThread::run()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   while (true)
   {
      if ( _switch == 1 )
      {
         // Process this object's saved work block, saving the result block and
         // transferring it to this object's main thread. If any ACE exception occurs then
         // catch it and save it.
         try
         {
            _result = _worker->execute(_work).release();
            _result->moveToThread(thread());
            _result->setParent(this);
         }
         catch (EException e)
         {
            _exception = new EException(e);
         }
         --_switch;
         emit blockFinished();
      }
      else if ( isInterruptionRequested() )
      {
         return;
      }
      usleep(10);
      if ( _switch == 0 )
      {
         msleep(10);
      }
   }
}

}
}
