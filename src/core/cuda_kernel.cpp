#include "cuda_kernel.h"
#include "cuda_event.h"
#include "cuda_program.h"



using namespace CUDA;
//






/*!
 * Constructs a new kernel object from the given program with the given kernel name.
 *
 * @param program Pointer to the program which has built the kernel with the given
 *                name that is created.
 *
 * @param name The name of the kernel that is created.
 */
Kernel::Kernel(Program* program, const QString& name)
{
   // Create a new CUDA kernel from the given program with the given name, storing 
   // its id to this object. If creating the kernel fails then throw an exception. 
   CUresult result = cuModuleGetFunction(&_kernel, program->id(), name.toLatin1().data());
   if ( result != CUDA_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      throwError(&e,result);
   }
}






/*!
 * Executes this object's CUDA kernel on the given stream with the global sizes,
 * local sizes, and arguments of this object, returning the event for the kernel
 * command.
 *
 * @param stream CUDA stream on which to execute this kernel.
 *
 * @return The event for the kernel command running on the given stream.
 */
Event Kernel::execute(const Stream& stream)
{
   // Launch this kernel on the given stream using this object's work sizes and
   // kernel arguments. If the kernel launch fails then throw an exception.
   CUresult result
   {
      cuLaunchKernel(_kernel
                     ,_gridDim.x
                     ,_gridDim.y
                     ,_gridDim.z
                     ,_blockDim.x
                     ,_blockDim.y
                     ,_blockDim.z
                     ,_sharedMemBytes
                     ,stream.id()
                     ,_args.data()
                     ,nullptr)
   };
   if ( result != CUDA_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      throwError(&e,result);
   }

   // Record the enqueued kernel launch to an event and return the event.
   Event event;
   event.record(stream);

   return event;
}






/*!
 * Returns an attribute of this CUDA kernel. Refer to the CUDA Toolkit Documentation
 * for more information on the available kernel attributes.
 *
 * @param attribute Enumerated value for a CUDA kernel attribute.
 */
int Kernel::getAttribute(CUfunction_attribute attribute) const
{
   // Get the value of the given kernel attribute. If this command fails then
   // throw an exception.
   int value;
   CUresult result = cuFuncGetAttribute(&value, attribute, _kernel);
   if ( result != CUDA_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      throwError(&e,result);
   }

   return value;
}






/*!
 * Sets the global and local work sizes used for parallel execution of this kernel
 * object. Each work size can be one-, two-, or three-dimensional.
 *
 * @param globalSize Global work size, also known as grid size.
 *
 * @param localSize Local work size. also known as block size.
 */
void Kernel::setSizes(dim3 globalSize, dim3 localSize)
{
   _gridDim = globalSize;
   _blockDim = localSize;
}
