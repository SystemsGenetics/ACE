#ifndef ACCELCOMPENG_CLEVENT_H
#define ACCELCOMPENG_CLEVENT_H
#include <CL/cl.h>
#include "exception.h"
namespace AccelCompEng
{



class CLEvent
{
public:
   ACE_EXCEPTION(AccelCompEng::CLEvent,CannotWait)
   ACE_EXCEPTION(AccelCompEng::CLEvent,CannotGetInfo)
   ACE_EXCEPTION(AccelCompEng::CLEvent,ExecutionFail)
   friend class CLCommandQueue;
   CLEvent() = default;
   ~CLEvent();
   CLEvent(const CLEvent&) = delete;
   CLEvent& operator=(const CLEvent&) = delete;
   CLEvent(CLEvent&&);
   CLEvent& operator=(CLEvent&&);
   void wait();
   bool is_done();
private:
   CLEvent(cl_event);
   bool _hasEvent {false};
   cl_event _id;
};



}
#endif
