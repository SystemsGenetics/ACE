#include "ace_analytic_openclrun.h"
#include <QSignalMapper>
#include "ace_analytic_openclrun_thread.h"
#include "ace_analytic_abstractinput.h"
#include "ace_settings.h"
#include "eabstractanalytic_opencl_worker.h"
#include "eabstractanalytic_block.h"
#include "opencl_device.h"
#include "opencl_context.h"
#include "eexception.h"



using namespace std;
using namespace Ace::Analytic;
//






/*!
 *
 * @param opencl  
 *
 * @param device  
 *
 * @param base  
 *
 * @param parent  
 */
OpenCLRun::OpenCLRun(EAbstractAnalytic::OpenCL* opencl, OpenCL::Device* device, AbstractInput* base, QObject* parent):
   AbstractRun(parent),
   _context(new OpenCL::Context({device},this)),
   _opencl(opencl),
   _base(base),
   _threads(Settings::instance().threadSize())
{
   opencl->initialize(_context);
   QSignalMapper* mapper {new QSignalMapper(this)};
   connect(mapper,QOverload<int>::of(&QSignalMapper::mapped),this,&OpenCLRun::blockFinished);
   for (int i = 0; i < _threads.size() ;++i)
   {
      Thread* thread {new Thread(_opencl->makeWorker())};
      _threads[i] = thread;
      _idle << thread;
      mapper->setMapping(thread,i);
      connect(thread
              ,&Thread::finished
              ,mapper
              ,QOverload<>::of(&QSignalMapper::map)
              ,Qt::QueuedConnection);
   }
}






/*!
 */
OpenCLRun::~OpenCLRun()
{
   for (auto thread: _threads)
   {
      while ( thread->isRunning() );
      delete thread;
   }
}






/*!
 *
 * @param block  
 */
void OpenCLRun::addWork(std::unique_ptr<EAbstractAnalytic::Block>&& block)
{
   while ( _idle.isEmpty() )
   {
      QCoreApplication::processEvents();
   }
   _idle.dequeue()->execute(std::move(block));
}






/*!
 *
 * @param index  
 */
void OpenCLRun::blockFinished(int index)
{
   Thread* thread {_threads.at(index)};
   _base->saveResult(thread->result());
   _idle.enqueue(thread);
   if ( _base->isFinished() )
   {
      emit finished();
   }
}
