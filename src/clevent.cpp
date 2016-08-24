#include "clevent.h"
namespace AccelCompEng
{



CLEvent::~CLEvent()
{
   if (_hasEvent)
   {
      clReleaseEvent(_id);
   }
}



CLEvent::CLEvent(CLEvent&& move):
   _hasEvent(move._hasEvent),
   _id(move._id)
{
   move._hasEvent = false;
}



CLEvent& CLEvent::operator=(CLEvent&& move)
{
   if (_hasEvent)
   {
      clReleaseEvent(_id);
   }
   _hasEvent = move._hasEvent;
   _id = move._id;
   move._hasEvent = false;
}



void CLEvent::wait()
{
   static const char* f = __PRETTY_FUNCTION__;
   if (_hasEvent)
   {
      cl_int err;
      err = clWaitForEvents(1,&_id);
      classert<CannotWait>(err,f,__LINE__);
   }
}



bool CLEvent::is_done()
{
   static const char* f = __PRETTY_FUNCTION__;
   bool ret {true};
   if (_hasEvent)
   {
      cl_int status;
      cl_int err;
      err = clGetEventInfo(_id,CL_EVENT_COMMAND_EXECUTION_STATUS,sizeof(cl_int),
                           &status,NULL);
      classert<CannotGetInfo>(err,f,__LINE__);
      assert<ExecutionFail>(status>=0,f,__LINE__);
      if (status!=CL_COMPLETE)
      {
         ret = false;
      }
   }
   return ret;
}



CLEvent::CLEvent(cl_event id):
   _hasEvent(true),
   _id(id)
{}



}
