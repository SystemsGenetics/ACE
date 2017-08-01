#ifndef EOPENCLEVENT_H
#define EOPENCLEVENT_H
#include <CL/cl.h>

#include "opencl.h"



/// Can hold a single OpenCL event or not event.
class EOpenCLEvent : public EOpenCL
{
public:
   /// Creates a new opencl object with no attached event.
   EOpenCLEvent() = default;
   /// Internal command DO NOT USE.
   EOpenCLEvent(cl_event id);
   ~EOpenCLEvent();
   EOpenCLEvent(const EOpenCLEvent& copy);
   EOpenCLEvent& operator=(const EOpenCLEvent& copy);
   EOpenCLEvent(EOpenCLEvent&& move);
   EOpenCLEvent& operator=(EOpenCLEvent&& move);
   /// Blocks execution until object's event is complete.
   void wait();
   /// Determine if object's event is complete.
   ///
   /// @return True if object's event is complete.
   bool isDone();
   /// Determine if object holds an event.
   ///
   /// @return True if object holds no event.
   bool isNull() const;
private:
   cl_event* _id {nullptr};
};



#endif
