#ifndef OPENCL_EVENT_H
#define OPENCL_EVENT_H
#include <CL/cl.h>
//



namespace OpenCL
{
   /*!
    * This can contain an OpenCL event or be set to null. An event this class contains 
    * can be copied or moved to another instance of this class. Copying an instance 
    * with an OpenCL event simply means copying the ID and calling the retain method. 
    * Once all instances referencing an OpenCL event are deleted the underlying event 
    * is discarded. This class also provides utility methods for testing or waiting on 
    * its event, along with testing to see if it is null. 
    */
   class Event
   {
   public:
      void operator=(const Event& other);
      void operator=(Event&& other);
   public:
      /*!
       * Constructs a new event that is set to null (no OpenCL event). 
       */
      Event() = default;
      Event(cl_event id);
      Event(const Event& other);
      Event(Event&& other);
      ~Event();
      bool isNull() const;
      void wait() const;
      bool isDone() const;
   private:
      void release();
      void retain();
      /*!
       * Pointer the OpenCL event ID of this object or null of this object is null and 
       * contains no event. 
       */
      cl_event* _id {nullptr};
   };
}



#endif
