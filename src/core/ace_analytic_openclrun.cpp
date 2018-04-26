#include "ace_analytic_openclrun.h"
#include <QSignalMapper>
#include "ace_analytic_openclrun_thread.h"
#include "ace_analytic_iobase.h"
#include "eabstractanalytic_opencl_block.h"
#include "eabstractanalytic_block.h"
#include "opencl_device.h"
#include "opencl_context.h"
#include "eexception.h"



using namespace std;
using namespace Ace::Analytic;
//



/*!
 */
const int OpenCLRun::_threadSize {8};






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
OpenCLRun::OpenCLRun(EAbstractAnalytic::OpenCL* opencl, OpenCL::Device* device, IOBase* base, QObject* parent):
   Run(parent),
   _context(new OpenCL::Context({device},this)),
   _opencl(opencl),
   _base(base),
   _threads(_threadSize)
{
   QSignalMapper* mapper {new QSignalMapper(this)};
   connect(mapper,QOverload<int>::of(&QSignalMapper::mapped),this,&OpenCLRun::blockFinished);
   for (int i = 0; i < _threadSize ;++i)
   {
      Thread* thread {new Thread(_opencl->makeBlock())};
      _threads[i] = thread;
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
 */
void OpenCLRun::start()
{
   for (auto thread: _threads)
   {
      if ( _base->hasWork() )
      {
         thread->execute(_base->makeWork());
      }
   }
}






/*!
 *
 * @param index  
 */
void OpenCLRun::blockFinished(int index)
{
   Thread* thread {_threads.at(index)};
   _base->saveResult(thread->result());
   if ( _base->isFinished() )
   {
      emit finished();
   }
   else if ( _base->hasWork() )
   {
      thread->execute(_base->makeWork());
   }
}
