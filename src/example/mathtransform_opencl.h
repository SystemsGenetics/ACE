#ifndef MATHTRANSFORM_OPENCL_H
#define MATHTRANSFORM_OPENCL_H
#include <core/openclxx.h>
#include "mathtransform.h"
//



/*!
 */
class MathTransform::OpenCL : public EAbstractAnalytic::OpenCL
{
   Q_OBJECT
public:
   class Kernel;
   class Worker;
   explicit OpenCL(EAbstractAnalytic* parent);
   virtual std::unique_ptr<EAbstractAnalytic::OpenCL::Worker> makeWorker() const override final;
   virtual void initialize(::OpenCL::Context* context) override final;
private:
   /*!
    */
   MathTransform* _base;
   /*!
    */
   ::OpenCL::Context* _context {nullptr};
   /*!
    */
   ::OpenCL::Program* _program {nullptr};
};



#endif
