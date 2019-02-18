#include "cuda_stream.h"
#include "cuda_event.h"



using namespace CUDA;
//






/*!
 * Constructs a new CUDA stream. If a CUDA error occurs then an exception is thrown. 
 */
Stream::Stream()
{
   // Create a new CUDA stream. If the command fails then throw an exception.
   CUDA_SAFE_CALL(cuStreamCreate(&_id, CU_STREAM_DEFAULT));
}






/*!
 * Constructs a CUDA stream by taking the state from another stream object.
 *
 * @param other CUDA stream object whose state is taken.
 */
Stream::Stream(Stream&& other)
   : Stream()
{
   // Take the state of the other stream object and give it the state of this
   // object. The previous state of this object will be properly destroyed with
   // the other stream object.
   std::swap(_id, other._id);
}






/*!
 * Releases the underlying CUDA stream resource. 
 */
Stream::~Stream()
{
   if ( _id != nullptr )
   {
      CUDA_SAFE_CALL(cuStreamDestroy(_id));
   }
}






/*!
 * Take the state from another stream object.
 *
 * @param other CUDA stream object whose state is taken.
 */
Stream& Stream::operator=(Stream&& other)
{
   // Take the state of the other stream object and give it the state of this
   // object. The previous state of this object will be properly destroyed with
   // the other stream object.
   std::swap(_id, other._id);
   return *this;
}






/*!
 * Returns a stream object for the default stream.
 *
 * @return Stream object for the default stream.
 */
CUDA::Stream Stream::getDefaultStream()
{
   return Stream(nullptr);
}






/*!
 * Waits until all operations in this stream are complete. If a CUDA error
 * occurs then an exception is thrown.
 */
void Stream::wait()
{
   CUDA_SAFE_CALL(cuStreamSynchronize(_id));
}






/*!
 * Blocks all future operations on this stream until the given event is complete.
 * If a CUDA error occurs then an exception is thrown.
 *
 * @param event CUDA event on which to block this stream.
 */
void Stream::waitEvent(const Event& event)
{
   CUDA_SAFE_CALL(cuStreamWaitEvent(_id, event.id(), 0));
}






/*!
 * Checks whether all operations in this stream are complete. If a CUDA error
 * occurs then an exception is thrown.
 *
 * @return True if all operations in this stream are complete, otherwise false.
 */
bool Stream::isDone()
{
   // Get the status of this object's stream. If this command returns a valid
   // status code then return true or false, otherwise throw an exception.
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
