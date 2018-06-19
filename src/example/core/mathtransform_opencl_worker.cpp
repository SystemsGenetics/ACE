#include "mathtransform_opencl_worker.h"
#include "mathtransform_opencl_kernel.h"
#include "mathtransform_block.h"



using namespace std;
//






/*!
 * Constructs a new OpenCL worker with the given math transform base, OpenCL 
 * context, and OpenCL program. 
 *
 * @param base Pointer to the base math transform analytic used to get argument 
 *             values from. 
 *
 * @param context Pointer to the OpenCL context used to create a buffer and command 
 *                queue. 
 *
 * @param program Pointer to OpenCL program used to create this new worker's kernel 
 *                object. 
 */
MathTransform::OpenCL::Worker::Worker(MathTransform* base, ::OpenCL::Context* context, ::OpenCL::Program* program):
   _base(base),
   _queue(new ::OpenCL::CommandQueue(context,context->devices().first(),this)),
   _kernel(new OpenCL::Kernel(program,this)),
   _buffer(context,1)
{}






/*!
 * Implements the interface that reads in the given work block, executes the 
 * algorithms necessary to produce its results using OpenCL acceleration, and saves 
 * those results in a new results block whose pointer is returned. 
 *
 * @param block Pointer to work block used to create a results block. 
 *
 * @return Pointer to results block produced from the given work block. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Cast the given generic work block to this analytic type work block _valid_. 
 *
 * 2. Map the integer buffer to host memory for writing, write the single integer 
 *    from _valid_, then unmap the integer buffer. Wait for all mapping to finish 
 *    before moving to the next step. 
 *
 * 3. Execute this object's kernel with its integer buffer, this base analytic 
 *    object's operation type, and its amount. Wait for the kernel to finish. 
 *
 * 4. Map the integer buffer to host memory for reading, read the transformed 
 *    single integer to _answer_, then unmap the integer buffer. Wait for all 
 *    mapping to finish before moving to the next step. 
 *
 * 5. Create a new result block with the given work block's index and the integer 
 *    _answer_, returning its pointer. 
 */
std::unique_ptr<EAbstractAnalytic::Block> MathTransform::OpenCL::Worker::execute(const EAbstractAnalytic::Block* block)
{
   // 1
   const MathTransform::Block* valid {block->cast<const MathTransform::Block>()};

   // 2
   _buffer.mapWrite(_queue).wait();
   _buffer[0] = valid->_number;
   _buffer.unmap(_queue).wait();

   // 3
   _kernel->execute(_queue,&_buffer,_base->_type,_base->_amount).wait();

   // 4
   _buffer.mapRead(_queue).wait();
   qint32 answer {_buffer.at(0)};
   _buffer.unmap(_queue).wait();

   // 5
   return unique_ptr<EAbstractAnalytic::Block>(new MathTransform::Block(block->index(),answer));
}
