#include "ace_analytic_openclengine_piston.h"
#include "eabstractanalytic_opencl_block.h"
#include "eabstractanalytic_block.h"
#include "eexception.h"



using namespace std;
using namespace Ace::Analytic;
//






/*!
 *
 * @param engine  
 *
 * @param parent  
 */
OpenCLEngine::Piston::Piston(std::unique_ptr<EAbstractAnalytic::OpenCL::Block>&& engine, QObject* parent):
   QThread(parent),
   _engine(engine.release())
{
   _engine->setParent(this);
}






/*!
 *
 * @param block  
 */
void OpenCLEngine::Piston::execute(std::unique_ptr<EAbstractAnalytic::Block>&& block)
{
   if ( _result )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Cannot execute OpenCL engine piston that contains a result."));
      throw e;
   }
   delete _work;
   _work = block.release();
   _work->setParent(this);
   start();
}






/*!
 */
std::unique_ptr<EAbstractAnalytic::Block> OpenCLEngine::Piston::result()
{
   if ( !_result )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Cannot get result from OpenCL engine piston that contains none."));
      throw e;
   }
   _result->setParent(nullptr);
   unique_ptr<EAbstractAnalytic::Block> ret {_result};
   _result = nullptr;
   return ret;
}






/*!
 */
void OpenCLEngine::Piston::run()
{
   _result = _engine->execute(_work).release();
   _result->setParent(this);
}
