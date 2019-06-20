#include "ace_analytic_openclrun.h"
#include "ace_analytic_openclrunthread.h"
#include "ace_analytic_abstractinput.h"
#include "ace_settings.h"
#include "opencl_device.h"
#include "opencl_context.h"
#include "eexception.h"
#include "edebug.h"
#include "eabstractanalyticblock.h"
#include "eabstractanalyticopencl.h"
#include "eabstractanalyticopenclworker.h"



namespace Ace
{
namespace Analytic
{






/*!
 * Implements the interface that is called to add a work block to be processed
 * by this abstract run. This implementation waits until a thread is idle and
 * then adding the given work block to the idle thread for execution. While this
 * waits it is blocking.
 *
 * @param block The work block that is processed.
 */
void OpenCLRun::addWork(std::unique_ptr<EAbstractAnalyticBlock>&& block)
{
   // Add the debug header.
   EDEBUG_FUNC(this,block.get());

   // Wait until there is at least one idle thread, blocking execution until that is
   // the case.
   while ( _idle.isEmpty() )
   {
      QCoreApplication::processEvents();
   }

   // Add the given work block to the first idle thread for execution in its own
   // thread.
   _idle.dequeue()->execute(std::move(block));
}






/*!
 * Constructs a new OpenCL run object with the given abstract OpenCL object,
 * OpenCL device, abstract input object, and optional parent.
 *
 * @param opencl Pointer to the abstract OpenCL object used for creation
 *               abstract OpenCL blocks.
 *
 * @param device Pointer to the OpenCL device utilized by this new OpenCL
 *               object.
 *
 * @param base Pointer to the abstract input object used to save results.
 *
 * @param parent Optional parent for this new OpenCL run.
 */
OpenCLRun::OpenCLRun(EAbstractAnalyticOpenCL* opencl, OpenCL::Device* device, AbstractInput* base, QObject* parent)
   :
   AbstractRun(parent),
   _context(new OpenCL::Context({device},this)),
   _opencl(opencl),
   _base(base),
   _threads(Settings::instance().threadSize())
{
   // Add the debug header.
   EDEBUG_FUNC(this,opencl,device,base,parent);

   // Initialize the given abstract OpenCL object, create a qt mapper and connect its
   // mapped signal.
   opencl->initialize(_context);

   // Iterate through the number of OpenCL threads this run object contains,
   // initializing each one with a new worker created from the abstract OpenCL
   // object. Add all threads to the idle queue, the mapper with their given index,
   // and connecting their finished signal with this object's block finished slot
   // using the mapper to get the index.
   for (int i = 0; i < _threads.size() ;++i)
   {
      OpenCLRunThread* thread {new OpenCLRunThread(_opencl->makeWorker())};
      _threads[i] = thread;
      _idle << thread;
      connect(thread
              ,&OpenCLRunThread::blockFinished
              ,this
              ,[this,i]{ blockFinished(i); }
              ,Qt::QueuedConnection);
      thread->start();
   }
}






/*!
 * This deletes all threads this OpenCL run contains. The threads are not
 * deleted until their thread has finished execution, blocking until that is the
 * case. This is to prevent undefined behavior.
 */
OpenCLRun::~OpenCLRun()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Iterate through all threads, for each one waiting until it is no longer running
   // and then deleting it.
   for (auto thread: _threads)
   {
      thread->requestInterruption();
      while ( thread->isRunning() );
      delete thread;
   }
}






/*!
 * Called when on of this object's threads has finished execution and contains a
 * result block.
 *
 */
void OpenCLRun::blockFinished(int index)
{
   // Add the debug header.
   EDEBUG_FUNC(this,index);

   // Get the result block from the thread that finished execution, saving it to this
   // object's abstract input and adding the thread to this object's idle queue.
   OpenCLRunThread* thread {_threads.at(index)};
   _base->saveResult(thread->result());
   _idle.enqueue(thread);

   // If this object's abstract input is finished then emit the finished signal.
   if ( _base->isFinished() )
   {
      emit finished();
   }
}

}
}
