#include "mathtransform_cuda.h"
#include "mathtransform_cuda_worker.h"



using namespace std;
//






/*!
 * Constructs a new CUDA object with the given math transform parent. 
 *
 * @param parent Pointer to the parent math transform analytic of this new CUDA 
 *               object. 
 */
MathTransform::CUDA::CUDA(MathTransform* parent):
   EAbstractAnalyticCUDA(parent),
   _base(parent)
{}






/*!
 * Implements the interface that creates and returns a new CUDA worker for the 
 * implementation's analytic type. 
 *
 * @return Pointer to a new CUDA worker object. 
 */
std::unique_ptr<EAbstractAnalyticCUDAWorker> MathTransform::CUDA::makeWorker()
{
   return unique_ptr<EAbstractAnalyticCUDAWorker>(new Worker(_base,this,_program));
}






/*!
 * Implements the interface that initializes all CUDA resources used by this 
 * object's implementation. 
 *
 * @param context The CUDA context to used to initialize all other CUDA 
 *                resources. 
 */
void MathTransform::CUDA::initialize(::CUDA::Context* context)
{
   // Set this object's context pointer to the one given and then create this 
   // object's CUDA program. 
   _context = context;
   _program = new ::CUDA::Program({":/cuda/mathtransform.cu"},this);
}
