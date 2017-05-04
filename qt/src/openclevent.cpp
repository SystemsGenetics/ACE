#include "openclevent.h"
#include "utilities.h"






OpenCLEvent::OpenCLEvent(cl_event id)
{
   _id = new cl_event;
   *_id = id;
}






OpenCLEvent::~OpenCLEvent()
{
   if ( _id )
   {
      clReleaseEvent(*_id);
   }
   delete _id;
}






OpenCLEvent::OpenCLEvent(const OpenCLEvent& copy)
{
   try
   {
      // allocate new event and copy
      _id = new cl_event;
      *_id = *(copy._id);

      // retain copied event
      cl_int code = clRetainEvent(*_id);
      if ( code != CL_SUCCESS )
      {
         OpenCL::throwError("clRetainEvent",code);
      }
   }
   catch (...)
   {
      delete _id;
      throw;
   }
}






OpenCLEvent& OpenCLEvent::operator=(const OpenCLEvent& copy)
{
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
         OpenCL::throwError("clRetainEvent",code);
      }
   }

   // copy event and retain it
   *_id = *(copy._id);
   cl_int code = clRetainEvent(*_id);
   if ( code != CL_SUCCESS )
   {
      OpenCL::throwError("clRetainEvent",code);
   }
   return *this;
}






OpenCLEvent::OpenCLEvent(OpenCLEvent&& move)
{
   try
   {
      _id = new cl_event;
      _id = move._id;
      move._id = nullptr;
   }
   catch (...)
   {
      delete _id;
      throw;
   }
}






OpenCLEvent& OpenCLEvent::operator=(OpenCLEvent&& move)
{
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
         OpenCL::throwError("clRetainEvent",code);
      }
   }

   // move event
   _id = move._id;
   move._id = nullptr;
   return *this;
}






void OpenCLEvent::wait() const
{
   if ( _id )
   {
      cl_int code = clWaitForEvents(1,_id);
      if ( code != CL_SUCCESS )
      {
         OpenCL::throwError("clWaitForEvents",code);
      }
   }
}






bool OpenCLEvent::isDone() const
{
   // return if there is no event
   if ( !_id )
   {
      return false;
   }

   // query event status
   cl_int status;
   cl_int code = clGetEventInfo(*_id,CL_EVENT_COMMAND_EXECUTION_STATUS,sizeof(cl_int),&status
                                ,nullptr);
   if ( code != CL_SUCCESS )
   {
      OpenCL::throwError("clGetEventInfo",code);
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
