#include "mathtransform_opencl.h"
#include "mathtransform_opencl_worker.h"



using namespace std;
//






/*!
 * Constructs a new OpenCL object with the given math transform parent. 
 *
 * @param parent Pointer to the parent math transform analytic of this new OpenCL 
 *               object. 
 */
MathTransform::OpenCL::OpenCL(MathTransform* parent):
   EAbstractAnalytic::OpenCL(parent),
   _base(parent)
{}






/*!
 * Implements the interface that creates and returns a new OpenCL worker for the 
 * implementation's analytic type. 
 *
 * @return Pointer to a new OpenCL worker object. 
 */
std::unique_ptr<EAbstractAnalytic::OpenCL::Worker> MathTransform::OpenCL::makeWorker()
{
   return unique_ptr<EAbstractAnalytic::OpenCL::Worker>(new Worker(_base,_context,_program));
}






/*!
 * Implements the interface that initializes all OpenCL resources used by this 
 * object's implementation. 
 *
 * @param context The OpenCL context to used to initialize all other OpenCL 
 *                resources. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Set this object's context pointer to the one given and then create this 
 *    object's OpenCL program. 
 */
void MathTransform::OpenCL::initialize(::OpenCL::Context* context)
{
   // 1
   _context = context;
   _program = new ::OpenCL::Program(context,{":/opencl/mathtransform.cl"},this);
}
