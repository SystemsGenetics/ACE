#include "cuda_event.h"



using namespace CUDA;
//






/*!
 * Constructs a new event.
 */
Event::Event()
{
   // Create a new event and assign it to this object's state. If creation fails
   // then throw an exception.
   CUDA_SAFE_CALL(cuEventCreate(&_id, CU_EVENT_DEFAULT));
}






/*!
 * Constructs a new event taking the CUDA event or null state of the given event.
 *
 * @param other The other event object whose data is taken and set to this new 
 *              event.
 */
Event::Event(Event&& other):
   _id(other._id)
{
   other._id = nullptr;
}






/*!
 * Releases the underlying CUDA event.
 */
Event::~Event()
{
   // Destroy this event object's state. If this command fails then throw an
   // exception.
   cuEventDestroy(_id);
}






/*!
 * Takes the CUDA event or null state of the given event object releasing any
 * CUDA event this event may currently contain.
 *
 * @param other The other event object whose CUDA event or null state is taken.
 */
Event& Event::operator=(Event&& other)
{
   // Take the state of the other event object and give it the state of this
   // event object. The former state of this event object will be properly
   // destroyed with the other object.
   std::swap(_id, other._id);
   return *this;
}






/*!
 * Records the contents of a CUDA stream into this event. After calling this
 * method, this event will not be done until the commands currently enqueued in
 * the given stream are completed.
 *
 * @param stream CUDA stream to record.
 */
void Event::record(const Stream& stream)
{
   // Record the contents of the given stream to this event. If this command
   // fails then throw an exception.
   CUDA_SAFE_CALL(cuEventRecord(_id, stream.id()));
}






/*!
 * Waits for this object's CUDA event to complete, blocking until it does. If
 * this event is null then this returns immediately.
 */
void Event::wait() const
{
   // Wait for this object's event to complete. If waiting fails then throw an
   // exception.
   CUDA_SAFE_CALL(cuEventSynchronize(_id));
}






/*!
 * Tests if this object's CUDA event is complete.
 *
 * @return True if this event is complete or null, otherwise false.
 */
bool Event::isDone() const
{
   // Get the status of this object's event. If this command returns a valid
   // status code then return true or false, otherwise throw an exception.
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
 * @param start First CUDA event.
 *
 * @param end Second CUDA event.
 *
 * @return The elapsed time (ms) between the two events.
 */
float Event::getElapsedTime(Event& start, Event& end)
{
   // Get the elapsed time (ms) between the two given events. If this command
   // fails then throw an exception.
   float ms;
   CUDA_SAFE_CALL(cuEventElapsedTime(&ms, start._id, end._id));

   return ms;
}
