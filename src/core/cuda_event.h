#ifndef CUDA_EVENT_H
#define CUDA_EVENT_H
#include "cuda_common.h"
#include "cuda_stream.h"



namespace CUDA
{
   /*!
    * This can contain a CUDA event or be set to null.
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
      CUevent _id {nullptr};
   };
}



#endif
