#include "mathtransform_opencl_kernel.h"
//






/*!
 *
 * @param program  
 *
 * @param parent  
 */
MathTransform::OpenCL::Kernel::Kernel(::OpenCL::Program* program, QObject* parent):
   ::OpenCL::Kernel(program,"mathTransform",parent)
{}






/*!
 *
 * @param queue  
 *
 * @param buffer  
 *
 * @param type  
 *
 * @param amount  
 */
::OpenCL::Event MathTransform::OpenCL::Kernel::execute(::OpenCL::CommandQueue* queue, ::OpenCL::Buffer<cl_int>* buffer, Operation type, int amount)
{
   Locker locker {lock()};
   setBuffer(GlobalNum,buffer);
   setLocalMemory<cl_int>(LocalNum,1);
   setArgument(Type,static_cast<int>(type));
   setArgument(Amount,amount);
   return ::OpenCL::Kernel::execute(queue);
}
