#include "mathtransform_opencl_worker.h"
#include "mathtransform_opencl_kernel.h"
#include "mathtransform_block.h"



using namespace std;
//






/*!
 *
 * @param base  
 *
 * @param context  
 *
 * @param program  
 */
MathTransform::OpenCL::Worker::Worker(MathTransform* base, ::OpenCL::Context* context, ::OpenCL::Program* program):
   _base(base),
   _queue(new ::OpenCL::CommandQueue(context,context->devices().first(),this)),
   _kernel(new OpenCL::Kernel(program,this)),
   _buffer(context,1)
{}






/*!
 *
 * @param block  
 */
std::unique_ptr<EAbstractAnalytic::Block> MathTransform::OpenCL::Worker::execute(const EAbstractAnalytic::Block* block)
{
   const MathTransform::Block* valid {block->cast<const MathTransform::Block>()};
   _buffer.mapWrite(_queue).wait();
   _buffer[0] = valid->_number;
   _buffer.unmap(_queue).wait();
   _kernel->execute(_queue,&_buffer,_base->_type,_base->_amount).wait();
   _buffer.mapRead(_queue).wait();
   qint32 answer {_buffer.at(0)};
   _buffer.unmap(_queue).wait();
   return unique_ptr<EAbstractAnalytic::Block>(new MathTransform::Block(block->index(),answer));
}
