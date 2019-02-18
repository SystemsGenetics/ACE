#ifndef CUDA_STREAM_H
#define CUDA_STREAM_H
#include "cuda_common.h"
//



namespace CUDA
{
   /*!
    * This contains a CUDA stream. This is a very basic class that simply 
    * maintains ownership of an underlying stream. The main purpose of this 
    * class is to be used by other classes that add commands to its underlying
    * stream. Streams can be moved but not copied. This class also provides a
    * static method for accessing the default stream.
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

      /*!
       * The CUDA stream ID of this object. 
       */
      CUstream _id {nullptr};
   };
}



#endif
