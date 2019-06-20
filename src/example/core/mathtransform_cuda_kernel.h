#ifndef MATHTRANSFORM_CUDA_KERNEL_H
#define MATHTRANSFORM_CUDA_KERNEL_H
#include "mathtransform_cuda.h"
//



/*!
 * This is the primary and sole CUDA kernel used for the math transform analytic 
 * type. This kernel takes one integer, transforms it, and returns the result. This 
 * is a superficial and very inefficient implementation of CUDA kernels in 
 * general and should only be used as an example of how to use ACE. 
 */
class MathTransform::CUDA::Kernel : public ::CUDA::Kernel
{
public:
   /*!
    * Defines the arguments passed to the CUDA kernel. 
    */
   enum Argument
   {
      /*!
       * Defines the global buffer argument. 
       */
      GlobalBuffer
      /*!
       * Defines the global size argument. 
       */
      ,GlobalSize
      /*!
       * Defines the operation type argument. 
       */
      ,Type
      /*!
       * Defines the amount argument. 
       */
      ,Amount
   };
   Kernel(::CUDA::Program* program);
   ::CUDA::Event execute(const ::CUDA::Stream& stream, ::CUDA::Buffer<float>* buffer, Operation type, int amount);
};



#endif
