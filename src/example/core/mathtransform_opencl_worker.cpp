#include "mathtransform_opencl_worker.h"
#include "mathtransform_opencl_kernel.h"
#include "mathtransform_block.h"
#include "core/elog.h"



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
   _buffer(context,base->_in->columnSize())
{}






/*!
 * Implements the interface that reads in the given work block, executes the 
 * algorithms necessary to produce its results using OpenCL acceleration, and saves 
 * those results in a new results block whose pointer is returned. 
 *
 * @param block Pointer to work block used to create a results block. 
 *
 * @return Pointer to results block produced from the given work block. 
 */
std::unique_ptr<EAbstractAnalyticBlock> MathTransform::OpenCL::Worker::execute(const EAbstractAnalyticBlock* block)
{
   if ( ELog::isActive() )
   {
      ELog() << tr("Executing(OpenCL) work index %1.").arg(block->index());
   }
   // Cast the given generic work block to this analytic type work block _valid_. 
   const MathTransform::Block* valid {block->cast<const MathTransform::Block>()};

   // Map the row buffer to host memory for writing, write the row data 
   // from _valid_, then unmap the row buffer. Wait for all mapping to finish 
   // before moving to the next step. 
   _buffer.mapWrite(_queue).wait();
   for ( int i = 0; i < valid->_data.size(); i++ )
   {
      _buffer[i] = valid->_data[i];
   }
   _buffer.unmap(_queue).wait();

   // Execute this object's kernel with its row buffer, this base analytic 
   // object's operation type, and its amount. Wait for the kernel to finish. 
   _kernel->execute(_queue,&_buffer,_base->_type,_base->_amount).wait();

   // Map the row buffer to host memory for reading, read the transformed single 
   // integer to _answer_, then unmap the integer buffer. Wait for all mapping to 
   // finish before moving to the next step. 
   QVector<float> row(valid->_data.size());

   _buffer.mapRead(_queue).wait();
   for ( int i = 0; i < row.size(); i++ )
   {
      row[i] = _buffer.at(i);
   }
   _buffer.unmap(_queue).wait();

   // Create a new result block with the given work block's index and 
   // _row_, returning its pointer. 
   return unique_ptr<EAbstractAnalyticBlock>(new MathTransform::Block(block->index(),row.size(),row.data()));
}
