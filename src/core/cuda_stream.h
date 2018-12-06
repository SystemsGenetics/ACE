#ifndef CUDA_STREAM_H
#define CUDA_STREAM_H
#include "cuda_common.h"



namespace CUDA
{
   /*!
    * This contains a CUDA stream.
    */
   class Stream
   {
   public:
      Stream();
      Stream(const Stream&) = delete;
      Stream(Stream&& move);
      ~Stream();
      Stream& operator=(Stream&& move);
      static Stream getDefaultStream();
      CUstream id() const { return _id; }
      void wait();
      void waitEvent(const Event& event);
      bool isDone();
   private:
      Stream(CUstream id) : _id(id) {}

      CUstream _id {nullptr};
   };
}



#endif
