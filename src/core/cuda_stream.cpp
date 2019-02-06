#include "cuda_stream.h"
#include "cuda_event.h"



using namespace CUDA;






Stream::Stream()
{
   CUDA_SAFE_CALL(cuStreamCreate(&_id, CU_STREAM_DEFAULT));
}






Stream::Stream(Stream&& other)
   : Stream()
{
   std::swap(_id, other._id);
}






Stream::~Stream()
{
   if ( _id != nullptr )
   {
      CUDA_SAFE_CALL(cuStreamDestroy(_id));
   }
}






Stream& Stream::operator=(Stream&& other)
{
   std::swap(_id, other._id);
   return *this;
}






/*!
 * Get the default stream.
 */
CUDA::Stream Stream::getDefaultStream()
{
   return Stream(nullptr);
}






/*!
 * Block host execution until all operations in stream are complete.
 */
void Stream::wait()
{
   CUDA_SAFE_CALL(cuStreamSynchronize(_id));
}






/*!
 * Block future stream operations until event is complete.
 *
 * @param event
 */
void Stream::waitEvent(const Event& event)
{
   CUDA_SAFE_CALL(cuStreamWaitEvent(_id, event.id(), 0));
}






/*!
 * Check whether all operations in stream are complete.
 */
bool Stream::isDone()
{
   // query stream status
   CUresult result = cuStreamQuery(_id);
   if ( result == CUDA_SUCCESS )
   {
      return true;
   }
   else if ( result == CUDA_ERROR_NOT_READY )
   {
      return false;
   }
   else
   {
      CUDA_THROW_ERROR(result);
   }
}
