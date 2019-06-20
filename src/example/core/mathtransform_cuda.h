#ifndef MATHTRANSFORM_CUDA_H
#define MATHTRANSFORM_CUDA_H
#include <core/cudaxx.h>
#include "mathtransform.h"
//



/*!
 * This implements the base CUDA class for its parent math transform analytic. 
 */
class MathTransform::CUDA : public EAbstractAnalyticCUDA
{
   Q_OBJECT
public:
   class Kernel;
   class Worker;
   explicit CUDA(MathTransform* parent);
   virtual std::unique_ptr<EAbstractAnalyticCUDAWorker> makeWorker() override final;
   virtual void initialize() override final;
private:
   /*!
    * Pointer to this object's parent math transform analytic. 
    */
   MathTransform* _base;
   /*!
    * Pointer to this object's CUDA program. 
    */
   ::CUDA::Program* _program {nullptr};
};



#endif
