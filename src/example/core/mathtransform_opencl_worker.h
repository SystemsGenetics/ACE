#ifndef MATHTRANSFORM_OPENCL_WORKER_H
#define MATHTRANSFORM_OPENCL_WORKER_H
#include "mathtransform_opencl.h"
//



/*!
 * This implements the OpenCL worker class for its parent math transform analytic. 
 */
class MathTransform::OpenCL::Worker : public EAbstractAnalytic::OpenCL::Worker
{
   Q_OBJECT
public:
   explicit Worker(MathTransform* base, ::OpenCL::Context* context, ::OpenCL::Program* program);
   virtual std::unique_ptr<EAbstractAnalytic::Block> execute(const EAbstractAnalytic::Block* block) override final;
private:
   /*!
    * Pointer to this worker's math transform analytic. 
    */
   MathTransform* _base;
   /*!
    * Pointer to this worker's unique and private command queue. 
    */
   ::OpenCL::CommandQueue* _queue;
   /*!
    * Pointer to this worker's unique and private kernel object. 
    */
   OpenCL::Kernel* _kernel;
   /*!
    * Pointer to this worker's unique and private row buffer. 
    */
   ::OpenCL::Buffer<cl_float> _buffer;
};



#endif
