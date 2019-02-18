#ifndef CUDA_EVENT_H
#define CUDA_EVENT_H
#include "cuda_common.h"
#include "cuda_stream.h"



namespace CUDA
{
   /*!
    * This can contain a CUDA event or be set to null. Event objects may be moved
    * but not copied. The primary way to use a CUDA event is to create a new event
    * and then "record" a stream on the event. Doing so will cause the event to
    * not be done until the currently enqueued commands on that stream are also
    * done. This class also provides utility methods for testing or waiting on
    * its event, along with measuring the elapsed time between two events.
    */
   class Event
   {
   public:
      Event();
      Event(const Event& other) = delete;
      Event(Event&& other);
      ~Event();
      Event& operator=(Event&& other);
      CUevent id() const { return _id; }
      void record(const Stream& stream);
      void wait() const;
      bool isDone() const;
      static float getElapsedTime(Event& start, Event& end);
   private:
      /*!
       * CUDA event ID of this object or null of this object is uninitialized. 
       */
      CUevent _id {nullptr};
   };
}



#endif
