#ifndef MATHTRANSFORM_OPENCL_WORKER_H
#define MATHTRANSFORM_OPENCL_WORKER_H
#include "mathtransform_opencl.h"
//



/*!
 */
class MathTransform::OpenCL::Worker : public EAbstractAnalytic::OpenCL::Worker
{
   Q_OBJECT
public:
   explicit Worker(MathTransform* base, ::OpenCL::Context* context, ::OpenCL::Program* program);
   virtual std::unique_ptr<EAbstractAnalytic::Block> execute(const EAbstractAnalytic::Block* block) override final;
private:
   /*!
    */
   MathTransform* _base;
   /*!
    */
   ::OpenCL::CommandQueue* _queue;
   /*!
    */
   OpenCL::Kernel* _kernel;
   /*!
    */
   ::OpenCL::Buffer<cl_int> _buffer;
};



#endif
