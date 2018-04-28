#ifndef MATHTRANSFORM_OPENCL_KERNEL_H
#define MATHTRANSFORM_OPENCL_KERNEL_H
#include "mathtransform_opencl.h"
//



/*!
 */
class MathTransform::OpenCL::Kernel : public ::OpenCL::Kernel
{
   Q_OBJECT
public:
   /*!
    */
   enum Argument
   {
      /*!
       */
      GlobalNum
      /*!
       */
      ,LocalNum
      /*!
       */
      ,Type
      /*!
       */
      ,Amount
   };
   explicit Kernel(::OpenCL::Program* program, QObject* parent = nullptr);
   ::OpenCL::Event execute(::OpenCL::CommandQueue* queue, ::OpenCL::Buffer<cl_int>* buffer, Operation type, int amount);
};



#endif
