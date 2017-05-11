#ifndef EOPENCLEVENT_H
#define EOPENCLEVENT_H
#include <CL/cl.h>



class EOpenCLEvent
{
public:
   EOpenCLEvent() = default;
   EOpenCLEvent(cl_event id);
   ~EOpenCLEvent();
   EOpenCLEvent(const EOpenCLEvent& copy);
   EOpenCLEvent& operator=(const EOpenCLEvent& copy);
   EOpenCLEvent(EOpenCLEvent&& move);
   EOpenCLEvent& operator=(EOpenCLEvent&& move);
   void wait() const;
   bool isDone() const;
private:
   cl_event* _id {nullptr};
};



#endif
