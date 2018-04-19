#ifndef OPENCL_EVENT_H
#define OPENCL_EVENT_H
#include <CL/cl.h>
//



namespace OpenCL
{
   /*!
    */
   class Event
   {
   public:
      /*!
       */
      Event() = default;
      Event(cl_event id);
      Event(const Event& other);
      Event(Event&& other);
      ~Event();
      void operator=(const Event& other);
      void operator=(Event&& other);
      void wait();
      bool isDone();
      bool isNull();
   private:
      /*!
       */
      cl_event* _id {nullptr};
   };
}



#endif
