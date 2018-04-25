#include "ace_analytic_openclengine.h"
#include <QSignalMapper>
#include "ace_analytic_openclengine_piston.h"
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
const int OpenCLEngine::_pistonSize {8};






/*!
 *
 * @param opencl  
 *
 * @param device  
 *
 * @param parent  
 */
OpenCLEngine::OpenCLEngine(std::unique_ptr<EAbstractAnalytic::OpenCL>&& opencl, OpenCL::Device* device, QObject* parent):
   QObject(parent),
   _context(new OpenCL::Context({device},this)),
   _opencl(opencl.release())
{
   _opencl->setParent(this);
   _pistons.reserve(_pistonSize);
   _idle.reserve(_pistonSize);
   QSignalMapper* mapper {new QSignalMapper(this)};
   connect(mapper,QOverload<int>::of(&QSignalMapper::mapped),this,&OpenCLEngine::blockFinished);
   for (int i = 0; i < _pistonSize ;++i)
   {
      Piston* piston {new Piston(_opencl->makeBlock(),this)};
      _pistons << piston;
      _idle << piston;
      mapper->setMapping(piston,i);
      connect(piston
              ,&Piston::finished
              ,mapper
              ,QOverload<>::of(&QSignalMapper::map)
              ,Qt::QueuedConnection);
   }
}






/*!
 */
bool OpenCLEngine::isFull() const
{
   return _idle.isEmpty();
}






/*!
 */
bool OpenCLEngine::hasResults() const
{
   return !_results.isEmpty();
}






/*!
 *
 * @param block  
 */
void OpenCLEngine::execute(std::unique_ptr<EAbstractAnalytic::Block>&& block)
{
   if ( _idle.isEmpty() )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Cannot execute new analytic block in OpenCL Engine when no pistons are idle."));
      throw e;
   }
   _idle.dequeue()->execute(std::move(block));
}






/*!
 */
std::unique_ptr<EAbstractAnalytic::Block> OpenCLEngine::result()
{
   if ( _results.isEmpty() )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Cannot get result from OpenCL engine when it contains none."));
      throw e;
   }
   EAbstractAnalytic::Block* result {_results.dequeue()};
   result->setParent(nullptr);
   return unique_ptr<EAbstractAnalytic::Block>(result);
}






/*!
 *
 * @param index  
 */
void OpenCLEngine::blockFinished(int index)
{
   EAbstractAnalytic::Block* result {_pistons.at(index)->result().release()};
   result->setParent(this);
   _results.enqueue(result);
   _idle.enqueue(_pistons.at(index));
   emit finished();
}
