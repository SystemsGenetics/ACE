#include "openclevent.h"
#include "utilities.h"






EOpenCLEvent::EOpenCLEvent(cl_event id)
{
   _id = new cl_event;
   *_id = id;
}






EOpenCLEvent::~EOpenCLEvent()
{
   // if event exists release it
   if ( _id )
   {
      clReleaseEvent(*_id);
   }

   // delete pointer
   delete _id;
}






EOpenCLEvent::EOpenCLEvent(const EOpenCLEvent& copy)
{
   // allocate new event and copy
   _id = new cl_event;
   *_id = *(copy._id);

   // retain copied event
   cl_int code = clRetainEvent(*_id);
   if ( code != CL_SUCCESS )
   {
      reportError("clRetainEvent",code);
      return;
   }
}






EOpenCLEvent& EOpenCLEvent::operator=(const EOpenCLEvent& copy)
{
   if ( !*this )
   {
      return *this;
   }
   // if there is no event then allocate one
   if ( !_id )
   {
      _id = new cl_event;
   }
   else
   {
      // if there is an event then release it
      cl_int code = clReleaseEvent(*_id);
      if ( code != CL_SUCCESS )
      {
         reportError("clRetainEvent",code);
         return *this;
      }
   }

   // copy event and retain it
   *_id = *(copy._id);
   cl_int code = clRetainEvent(*_id);
   if ( code != CL_SUCCESS )
   {
      reportError("clRetainEvent",code);
   }
   return *this;
}






EOpenCLEvent::EOpenCLEvent(EOpenCLEvent&& move)
{
   _id = new cl_event;
   _id = move._id;
   move._id = nullptr;
}






EOpenCLEvent& EOpenCLEvent::operator=(EOpenCLEvent&& move)
{
   if ( !*this )
   {
      return *this;
   }
   // if there is no event then allocate one
   if ( !_id )
   {
      _id = new cl_event;
   }
   else
   {
      // if there is an event then release it
      cl_int code = clReleaseEvent(*_id);
      if ( code != CL_SUCCESS )
      {
         reportError("clRetainEvent",code);
         return *this;
      }
   }

   // move event
   _id = move._id;
   move._id = nullptr;
   return *this;
}






void EOpenCLEvent::wait()
{
   if ( !*this )
   {
      return;
   }
   if ( _id )
   {
      cl_int code = clWaitForEvents(1,_id);
      if ( code != CL_SUCCESS )
      {
         reportError("clWaitForEvents",code);
      }
   }
}






bool EOpenCLEvent::isDone()
{
   if ( !*this )
   {
      return true;
   }
   // return if there is no event
   if ( !_id )
   {
      return true;
   }

   // query event status
   cl_int status;
   cl_int code = clGetEventInfo(*_id,CL_EVENT_COMMAND_EXECUTION_STATUS,sizeof(cl_int),&status
                                ,nullptr);
   if ( code != CL_SUCCESS )
   {
      reportError("clGetEventInfo",code);
      return true;
   }
   if ( status == CL_COMPLETE )
   {
      return true;
   }
   else
   {
      return false;
   }
}






bool EOpenCLEvent::isNull() const
{
   return !_id;
}
