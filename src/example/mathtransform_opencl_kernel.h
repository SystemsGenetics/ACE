#ifndef MATHTRANSFORM_OPENCL_KERNEL_H
#define MATHTRANSFORM_OPENCL_KERNEL_H
#include "mathtransform_opencl.h"
//



/*!
 * This is the primary and sole OpenCL kernel used for the math transform analytic 
 * type. This kernel takes one integer, transforms it, and returns the result. This 
 * is a superficial and very inefficient implementation of OpenCL kernels in 
 * general and should only be used as an example of how to use ACE. 
 */
class MathTransform::OpenCL::Kernel : public ::OpenCL::Kernel
{
   Q_OBJECT
public:
   /*!
    * Defines the arguments passed to the OpenCL kernel. 
    */
   enum Argument
   {
      /*!
       * Defines the global number argument. 
       */
      GlobalNum
      /*!
       * Defines the local number argument. 
       */
      ,LocalNum
      /*!
       * Defines the operation type argument. 
       */
      ,Type
      /*!
       * Defines the amount argument. 
       */
      ,Amount
   };
   explicit Kernel(::OpenCL::Program* program, QObject* parent = nullptr);
   ::OpenCL::Event execute(::OpenCL::CommandQueue* queue, ::OpenCL::Buffer<cl_int>* buffer, Operation type, int amount);
};



#endif
