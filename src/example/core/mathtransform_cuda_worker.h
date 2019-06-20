#ifndef MATHTRANSFORM_CUDA_WORKER_H
#define MATHTRANSFORM_CUDA_WORKER_H
#include "mathtransform_cuda.h"
#include "mathtransform_cuda_kernel.h"
//



/*!
 * This implements the CUDA worker class for its parent math transform analytic. 
 */
class MathTransform::CUDA::Worker : public EAbstractAnalyticCUDAWorker
{
   Q_OBJECT
public:
   explicit Worker(MathTransform* base, MathTransform::CUDA* baseCuda, ::CUDA::Program* program);
   virtual std::unique_ptr<EAbstractAnalyticBlock> execute(const EAbstractAnalyticBlock* block) override final;
private:
   /*!
    * Pointer to this worker's math transform analytic. 
    */
   MathTransform* _base;
   MathTransform::CUDA* _baseCuda;
   /*!
    * Unique stream object for this worker.
    */
   ::CUDA::Stream _stream;
   /*!
    * Unique kernel object for this worker.
    */
   CUDA::Kernel _kernel;
   /*!
    * Unique row buffer for this worker.
    */
   ::CUDA::Buffer<float> _buffer;
};



#endif
