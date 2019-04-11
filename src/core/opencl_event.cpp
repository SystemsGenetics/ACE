#include "opencl_event.h"
#include "opencl.h"
#include "eexception.h"
#include "edebug.h"



using namespace OpenCL;
//






/*!
 * Makes a copy of the given event object, releasing any OpenCL event this event 
 * may currently contain. 
 *
 * @param other The other event object that is copied to this event. 
 */
void Event::operator=(const Event& other)
{
   EDEBUG_FUNC(this,&other)

   // If this object contains an event ID then release it. 
   if ( _id )
   {
      release();
   }

   // If the given object is null then clear this object's event ID if any and exit, 
   // else go to the next step. 
   if ( other._id )
   {
      // If this object does not have an allocated event ID then allocate one. 
      if ( !_id )
      {
         _id = new cl_event;
      }

      // Copy and retain the given event object's OpenCL event ID. 
      *_id = *other._id;
      retain();
   }
   else
   {
      delete _id;
      _id = nullptr;
   }
}






/*!
 * Takes the OpenCL event or null state of the given event object releasing any 
 * OpenCL event this event may currently contain. 
 *
 * @param other The other event object whose OpenCL event or null state is taken. 
 */
void Event::operator=(Event&& other)
{
   EDEBUG_FUNC(this,&other)

   // If this object has an event ID then release and delete it. 
   if ( _id )
   {
      release();
      delete _id;
   }

   // Take the given even object's OpenCL event ID. 
   _id = other._id;
   other._id = nullptr;
}






/*!
 * Constructs a new event using the given OpenCL event ID. 
 *
 * @param id The OpenCL event ID that this new event contains. This must be a valid 
 *           OpenCL event that has one reference count. 
 */
Event::Event(cl_event id):
   _id(new cl_event)
{
   EDEBUG_FUNC(this,(void*)id)

   *_id = id;
}






/*!
 * Constructs a new event that is a copy of the given event. 
 *
 * @param other The other event object that is copied to this new event. 
 */
Event::Event(const Event& other)
{
   EDEBUG_FUNC(this,&other)

   // If the other event is null then do nothing and exit, else go to the next step. 
   if ( other._id )
   {
      // Allocate this event's ID pointer, copy the other event's ID to this one, and 
      // retain the event ID. 
      *_id = *other._id;
      retain();
   }
}






/*!
 * Constructs a new event taking the OpenCL event or null state of the given event. 
 *
 * @param other The other event object whose data is taken and set to this new 
 *              event. 
 */
Event::Event(Event&& other):
   _id(other._id)
{
   EDEBUG_FUNC(this,&other)

   other._id = nullptr;
}






/*!
 * Releases the underlying OpenCL event, reducing its reference count by one. 
 */
Event::~Event()
{
   EDEBUG_FUNC(this)

   // If this object has an allocated event ID then release the ID and delete it. 
   if ( _id )
   {
      clReleaseEvent(*_id);
      delete _id;
   }
}






/*!
 * Tests if this event is null and does not contain an OpenCL event. 
 *
 * @return True if this event is null or false otherwise. 
 */
bool Event::isNull() const
{
   EDEBUG_FUNC(this)

   return !_id;
}






/*!
 * Waits for this object's OpenCL event to complete, blocking until it does. If 
 * this event is null then this returns immediately. 
 */
void Event::wait() const
{
   EDEBUG_FUNC(this)

   // If this object is not null then wait for its event to complete. If waiting 
   // fails then throw an exception. 
   if ( _id )
   {
      cl_int code {clWaitForEvents(1,_id)};
      if ( code != CL_SUCCESS )
      {
         E_MAKE_EXCEPTION(e);
         fillException(&e,code);
         throw e;
      }
   }
}






/*!
 * Tests if this object's OpenCL event is complete. 
 *
 * @return True if this event is complete or null, otherwise false. 
 */
bool Event::isDone() const
{
   EDEBUG_FUNC(this)

   // If this event is null then return true, else go to the next step. 
   if ( !_id )
   {
      return true;
   }

   // Get the status of this object's OpenCL event. If the OpenCL call fails then 
   // throw an exception. 
   cl_int status;
   cl_int code
   {
      clGetEventInfo(*_id,CL_EVENT_COMMAND_EXECUTION_STATUS,sizeof(cl_int),&status,nullptr)
   };
   if ( code != CL_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      fillException(&e,code);
      throw e;
   }

   // Return true if this object's OpenCL event status is complete or false 
   // otherwise. 
   return status == CL_COMPLETE;
}






/*!
 * Reduces this object's OpenCL reference count by one. 
 */
void Event::release()
{
   EDEBUG_FUNC(this)

   // Release this object's OpenCL event, throwing an exception if it fails. 
   cl_int code {clRetainEvent(*_id)};
   if ( code != CL_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      fillException(&e,code);
      throw e;
   }
}






/*!
 * Increases this object's OpenCL reference count by one. 
 */
void Event::retain()
{
   EDEBUG_FUNC(this)

   // Retain this object's OpenCL event, throwing an exception if it fails. 
   cl_int code {clRetainEvent(*_id)};
   if ( code != CL_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      fillException(&e,code);
      throw e;
   }
}
