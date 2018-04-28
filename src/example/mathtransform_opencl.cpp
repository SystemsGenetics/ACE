#include "mathtransform_opencl.h"
#include "mathtransform_opencl_worker.h"



using namespace std;
//






/*!
 *
 * @param parent  
 */
MathTransform::OpenCL::OpenCL(EAbstractAnalytic* parent):
   EAbstractAnalytic::OpenCL(parent)
{}






/*!
 */
std::unique_ptr<EAbstractAnalytic::OpenCL::Worker> MathTransform::OpenCL::makeWorker() const
{
   return unique_ptr<EAbstractAnalytic::OpenCL::Worker>(new Worker(_base,_context,_program));
}






/*!
 *
 * @param context  
 */
void MathTransform::OpenCL::initialize(::OpenCL::Context* context)
{
   _context = context;
   _program = new ::OpenCL::Program(context,{":/opencl/mathtransform.cl"},this);
}
