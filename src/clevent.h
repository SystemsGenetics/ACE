#ifndef ACCELCOMPENG_CLEVENT_H
#define ACCELCOMPENG_CLEVENT_H
#include <CL/cl.h>
#include "exception.h"
namespace AccelCompEng
{


/// Wrapper for a single OpenCL event.
class CLEvent
{
public:
   struct CannotWait : public Exception { using Exception::Exception; };
   struct CannotGetInfo : public Exception { using Exception::Exception; };
   struct ExecutionFail : public Exception { using Exception::Exception; };
   friend class CLCommandQueue;
   /// Creates an empty or null object with no OpenCL event.
   CLEvent() = default;
   ~CLEvent();
   CLEvent(const CLEvent&) = delete;
   CLEvent& operator=(const CLEvent&) = delete;
   CLEvent(CLEvent&&);
   CLEvent& operator=(CLEvent&&);
   /// Blocks until the associated OpenCL event is complete.
   void wait();
   /// Returns status of OpenCL event.
   ///
   /// @return Has the OpenCL event reached completion?
   bool is_done();
private:
   /// Creates a new object with the given OpenCL event.
   ///
   /// @param id OpenCL event id.
   CLEvent(cl_event id);
   bool _hasEvent {false};
   cl_event _id;
};



}
#endif
