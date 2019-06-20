#include "mathtransform_cuda_kernel.h"



//






/*!
 * Constructs a new kernel object with the given CUDA program. 
 *
 * @param program Pointer to the CUDA program used to build this new kernel's 
 *                CUDA resource. 
 */
MathTransform::CUDA::Kernel::Kernel(::CUDA::Program* program):
   ::CUDA::Kernel(program,"mathTransform")
{}






/*!
 * Begins execution of this kernel object's CUDA kernel using the given CUDA 
 * stream and kernel arguments, returning the CUDA event associated with 
 * the kernel execution. 
 *
 * @param stream The CUDA stream this kernel is executed on. 
 *
 * @param buffer The CUDA memory buffer where a row is stored and will 
 *               be transformed by this kernel execution. 
 *
 * @param type The mathematical operation type that will be used for the transform. 
 *
 * @param amount The amount that will be used for the mathematical transform. 
 *
 * @return CUDA event associated with this kernel's execution. 
 */
::CUDA::Event MathTransform::CUDA::Kernel::execute(const ::CUDA::Stream& stream, ::CUDA::Buffer<float>* buffer, Operation type, int amount)
{
   // Set the arguments this CUDA kernel requires. This includes the global memory 
   // buffer where the row is held, the local memory buffer, the operation type, 
   // and the amount.
   setBuffer(GlobalBuffer,buffer);
   setArgument(GlobalSize,buffer->size());
   setArgument(Type,static_cast<int>(type));
   setArgument(Amount,amount);

   // Set the work sizes. The global work size is determined by the row size, but
   // it must also be a multiple of the local work size, so it is rounded up
   // accordingly.
   int localWorkSize = 1;
   int workgroupSize = (buffer->size() + localWorkSize - 1) / localWorkSize;

   setSizes(workgroupSize * localWorkSize, localWorkSize);

   // Execute this object's CUDA kernel with the given stream, returning its 
   // generated CUDA event. 
   return ::CUDA::Kernel::execute(stream);
}
