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
      bool isNull() const;
      void wait() const;
      bool isDone() const;
   private:
      /*!
       */
      cl_event* _id {nullptr};
   };
}



#endif
