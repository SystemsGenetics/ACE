#ifndef MATHTRANSFORM_OPENCL_H
#define MATHTRANSFORM_OPENCL_H
#include <core/openclxx.h>
#include "mathtransform.h"
//



/*!
 * This implements the base OpenCL class for its parent math transform analytic. 
 */
class MathTransform::OpenCL : public EAbstractAnalytic::OpenCL
{
   Q_OBJECT
public:
   class Kernel;
   class Worker;
   explicit OpenCL(MathTransform* parent);
   virtual std::unique_ptr<EAbstractAnalytic::OpenCL::Worker> makeWorker() override final;
   virtual void initialize(::OpenCL::Context* context) override final;
private:
   /*!
    * Pointer to this object's parent math transform analytic. 
    */
   MathTransform* _base;
   /*!
    * Pointer to this object's base OpenCL context used to create all other resources. 
    */
   ::OpenCL::Context* _context {nullptr};
   /*!
    * Pointer to this object's OpenCL program. 
    */
   ::OpenCL::Program* _program {nullptr};
};



#endif
