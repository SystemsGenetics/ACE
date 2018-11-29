#include "mathtransform_opencl_kernel.h"



//






/*!
 * Constructs a new kernel object with the given OpenCL program and optional 
 * parent. 
 *
 * @param program Pointer to the OpenCL program used to build this new kernel's 
 *                OpenCL resource. 
 *
 * @param parent Optional parent for this new kernel object. 
 */
MathTransform::OpenCL::Kernel::Kernel(::OpenCL::Program* program, QObject* parent):
   ::OpenCL::Kernel(program,"mathTransform",parent)
{}






/*!
 * Begins execution of this kernel object's OpenCL kernel using the given OpenCL 
 * command queue and kernel arguments, returning the OpenCL event associated with 
 * the kernel execution. 
 *
 * @param queue The OpenCL command queue this kernel is executed on. 
 *
 * @param buffer The OpenCL memory buffer where a row is stored and will 
 *               be transformed by this kernel execution. 
 *
 * @param type The mathematical operation type that will be used for the transform. 
 *
 * @param amount The amount that will be used for the mathematical transform. 
 *
 * @return OpenCL event associated with this kernel's execution. 
 */
::OpenCL::Event MathTransform::OpenCL::Kernel::execute(::OpenCL::CommandQueue* queue, ::OpenCL::Buffer<cl_float>* buffer, Operation type, int amount)
{
   // Lock this kernel's underlying kernel class so arguments can be set. 
   Locker locker {lock()};

   // Set the arguments this OpenCL kernel requires. This includes the global memory 
   // buffer where the row is held, the local memory buffer, the operation type, 
   // and the amount. 
   setBuffer(GlobalBuffer,buffer);
   setArgument(GlobalSize,buffer->size());
   setLocalMemory<cl_float>(LocalValue,1);
   setArgument(Type,static_cast<int>(type));
   setArgument(Amount,amount);

   // Set the work sizes. The global work size is determined by the row size, but
   // it must also be a multiple of the local work size, so it is rounded up
   // accordingly.
   int localWorkSize = 1;
   int workgroupSize = (buffer->size() + localWorkSize - 1) / localWorkSize;

   setSizes(0, workgroupSize * localWorkSize, localWorkSize);

   // Execute this object's OpenCL kernel with the given command queue, returning its 
   // generated OpenCL event. 
   return ::OpenCL::Kernel::execute(queue);
}
