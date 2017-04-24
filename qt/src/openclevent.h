#ifndef OPENCLEVENT_H
#define OPENCLEVENT_H
#include <CL/cl.h>



class OpenCLEvent
{
public:
   OpenCLEvent() = default;
   OpenCLEvent(cl_event id);
   ~OpenCLEvent();
   OpenCLEvent(const OpenCLEvent& copy);
   OpenCLEvent& operator=(const OpenCLEvent& copy);
   OpenCLEvent(OpenCLEvent&& move);
   OpenCLEvent& operator=(OpenCLEvent&& move);
   void wait() const;
   bool isDone() const;
private:
   cl_event* _id {nullptr};
};



#endif
