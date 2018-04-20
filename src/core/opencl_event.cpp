#include "opencl_event.h"
#include "opencl_common.h"
#include "eexception.h"



using namespace OpenCL;
//






/*!
 *
 * @param id  
 */
Event::Event(cl_event id):
   _id(new cl_event)
{
   *_id = id;
}






/*!
 *
 * @param other  
 */
Event::Event(const Event& other):
   _id(new cl_event)
{
   *_id = *other._id;
   cl_int code {clRetainEvent(*_id)};
   if ( code != CL_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      fillException(&e,code);
      throw e;
   }
}






/*!
 *
 * @param other  
 */
Event::Event(Event&& other):
   _id(other._id)
{
   other._id = nullptr;
}






/*!
 */
Event::~Event()
{
   if ( _id )
   {
      clReleaseEvent(*_id);
      delete _id;
   }
}






/*!
 *
 * @param other  
 */
void Event::operator=(const Event& other)
{
   if ( _id )
   {
      clReleaseEvent(*_id);
   }
   else
   {
      _id = new cl_event;
   }
   *_id = *other._id;
   cl_int code {clRetainEvent(*_id)};
   if ( code != CL_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      fillException(&e,code);
      throw e;
   }
}






/*!
 *
 * @param other  
 */
void Event::operator=(Event&& other)
{
   if ( _id )
   {
      clReleaseEvent(*_id);
      delete _id;
   }
   _id = other._id;
   other._id = nullptr;
}






/*!
 */
bool Event::isNull() const
{
   return !_id;
}






/*!
 */
void Event::wait() const
{
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
 */
bool Event::isDone() const
{
   if ( !_id )
   {
      return true;
   }
   cl_int status;
   cl_int code
   {//
      clGetEventInfo(*_id,CL_EVENT_COMMAND_EXECUTION_STATUS,sizeof(cl_int),&status,nullptr)
   };
   if ( code != CL_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      fillException(&e,code);
      throw e;
   }
   return status == CL_COMPLETE;
}
