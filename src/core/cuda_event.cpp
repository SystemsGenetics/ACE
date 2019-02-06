#include "cuda_event.h"



using namespace CUDA;






Event::Event()
{
   CUDA_SAFE_CALL(cuEventCreate(&_id, CU_EVENT_DEFAULT));
}






Event::Event(Event&& other):
   _id(other._id)
{
   other._id = nullptr;
}






Event::~Event()
{
   CUDA_SAFE_CALL(cuEventDestroy(_id));
}






Event& Event::operator=(Event&& other)
{
   std::swap(_id, other._id);
   return *this;
}






/*!
 * Record the contents of a CUDA stream into a CUDA event.
 *
 * @param stream
 */
void Event::record(const Stream& stream)
{
   CUDA_SAFE_CALL(cuEventRecord(_id, stream.id()));
}






/*!
 * Waits for this object's CUDA event to complete, blocking until it does. If
 * this event is null then this returns immediately.
 */
void Event::wait() const
{
   CUDA_SAFE_CALL(cuEventSynchronize(_id));
}






/*!
 * Tests if this object's CUDA event is complete.
 *
 * @return True if this event is complete or null, otherwise false.
 */
bool Event::isDone() const
{
   // query event status
   CUresult result = cuEventQuery(_id);
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






/*!
 * Get the elapsed time (ms) between two CUDA events.
 *
 * @param start
 * @param end
 */
float Event::getElapsedTime(Event& start, Event& end)
{
   float ms;
   CUDA_SAFE_CALL(cuEventElapsedTime(&ms, start._id, end._id));

   return ms;
}
