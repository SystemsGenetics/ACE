#include "eabstractanalytic_opencl.h"
#include "opencl_context.h"
//






/*!
 *
 * @param context  
 *
 * @param parent  
 */
EAbstractAnalytic::OpenCL::OpenCL(std::unique_ptr<::OpenCL::Context>&& context, EAbstractAnalytic* parent):
   QObject(parent),
   _context(context.get())
{
   context.release()->setParent(this);
}






/*!
 */
::OpenCL::Context& EAbstractAnalytic::OpenCL::context()
{
   return *_context;
}
