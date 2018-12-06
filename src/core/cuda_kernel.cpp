#include "cuda_kernel.h"
#include "cuda_event.h"
#include "cuda_program.h"



using namespace CUDA;






Kernel::Kernel(Program* program, const QString& name)
{
   CUDA_SAFE_CALL(cuModuleGetFunction(&_kernel, program->id(), name.toLatin1().data()));
}






/*!
 * Execute kernel on a CUDA stream.
 *
 * @param stream
 */
Event Kernel::execute(const Stream& stream)
{
   // launch kernel
   CUDA_SAFE_CALL(cuLaunchKernel(
      _kernel,
      _gridDim.x, _gridDim.y, _gridDim.z,
      _blockDim.x, _blockDim.y, _blockDim.z,
      _sharedMemBytes,
      stream.id(),
      _args.data(), nullptr));

   // return CUDA event of kernel execution
   Event event;
   event.record(stream);

   return event;
}






/*!
 * Get an attribute of a CUDA kernel.
 *
 * @param attribute
 */
int Kernel::getAttribute(CUfunction_attribute attribute) const
{
   int value;
   CUDA_SAFE_CALL(cuFuncGetAttribute(&value, attribute, _kernel));

   return value;
}






/*!
 * Sets the global and local sizes of the given dimension used for parallel 
 * execution of this kernel object.
 *
 * @param globalSize
 * @param localSize
 */
void Kernel::setSizes(dim3 globalSize, dim3 localSize)
{
   _gridDim = globalSize;
   _blockDim = localSize;
}
