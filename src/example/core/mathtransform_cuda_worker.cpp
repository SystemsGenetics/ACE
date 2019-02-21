#include "mathtransform_cuda_worker.h"
#include "mathtransform_block.h"
#include "core/elog.h"



using namespace std;
//






/*!
 * Constructs a new CUDA worker with the given math transform base and CUDA program.
 *
 * @param base Pointer to the base math transform analytic used to get argument
 *             values from.
 *
 * @param baseCuda Pointer to the base CUDA class used to access CUDA context.
 *
 * @param program Pointer to CUDA program used to create this new worker's kernel
 *                object.
 */
MathTransform::CUDA::Worker::Worker(MathTransform* base, MathTransform::CUDA* baseCuda, ::CUDA::Program* program):
   _base(base),
   _baseCuda(baseCuda),
   _kernel(program),
   _buffer(base->_in->columnSize())
{}






/*!
 * Implements the interface that reads in the given work block, executes the 
 * algorithms necessary to produce its results using CUDA acceleration, and saves
 * those results in a new results block whose pointer is returned.
 *
 * @param block Pointer to work block used to create a results block.
 *
 * @return Pointer to results block produced from the given work block.
 */
std::unique_ptr<EAbstractAnalytic::Block> MathTransform::CUDA::Worker::execute(const EAbstractAnalytic::Block* block)
{
   if ( ELog::isActive() )
   {
      ELog() << tr("Executing(CUDA) work index %1.").arg(block->index());
   }
   // Cast the given generic work block to this analytic type work block _valid_. 
   const MathTransform::Block* valid {block->cast<const MathTransform::Block>()};

   // Write the row data from _valid_ and wait for writing to finish.
   for ( int i = 0; i < valid->_data.size(); i++ )
   {
      _buffer[i] = valid->_data[i];
   }
   _buffer.write(_stream).wait();

   // Execute this object's kernel with its row buffer, this base analytic 
   // object's operation type, and its amount. Wait for the kernel to finish. 
   _kernel.execute(_stream,&_buffer,_base->_type,_base->_amount).wait();

   // Read the row data from the buffer and wait for reading to finish.
   _buffer.read(_stream).wait();

   // Create a new result block with the given work block's index and 
   // row data, returning its pointer. 
   return unique_ptr<EAbstractAnalytic::Block>(new MathTransform::Block(block->index(),_buffer.size(),_buffer.hostData()));
}
