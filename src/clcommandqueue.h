#ifndef ACCELCOMPENG_CLCOMMANDQUEUE_H
#define ACCELCOMPENG_CLCOMMANDQUEUE_H
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/cl.h>
#include "clkernel.h"
#include "clbuffer.h"
#include "clevent.h"
namespace AccelCompEng
{



class CLCommandQueue
{
public:
   struct CannotCreate : public Exception { using Exception::Exception; };
   struct CannotEnqueue : public Exception { using Exception::Exception; };
   struct CannotAddTask : public Exception { using Exception::Exception; };
   struct CannotAddSwarm : public Exception { using Exception::Exception; };
   struct NotInitialized : public Exception { using Exception::Exception; };
   struct NullBufferUsed : public Exception { using Exception::Exception; };
   struct NullKernelUsed : public Exception { using Exception::Exception; };
   CLCommandQueue() = default;
   ~CLCommandQueue();
   CLCommandQueue(const CLCommandQueue&) = delete;
   CLCommandQueue& operator=(const CLCommandQueue&) = delete;
   CLCommandQueue(CLCommandQueue&&) = delete;
   CLCommandQueue& operator=(CLCommandQueue&&) = delete;
   void init(cl_context,cl_device_id);
protected:
   template<class T> CLEvent read_buffer(CLBuffer<T>&);
   template<class T> CLEvent write_buffer(CLBuffer<T>&);
   CLEvent add_task(CLKernel&);
   CLEvent add_swarm(CLKernel&);
private:
   bool _initd {false};
   cl_command_queue _id;
};



template<class T> CLEvent CLCommandQueue::read_buffer(CLBuffer<T>& buffer)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NotInitialized>(_initd,f,__LINE__);
   assert<NullBufferUsed>(buffer._hostPtr,f,__LINE__);
   cl_event ret;
   cl_int err = clEnqueueReadBuffer(_id,buffer._id,CL_FALSE,0,
                                    buffer._size*sizeof(T),buffer._hostPtr,0,
                                    NULL,&ret);
   classert<CannotEnqueue>(err,f,__LINE__);
   return CLEvent(ret);
}



template<class T> CLEvent CLCommandQueue::write_buffer(CLBuffer<T>& buffer)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NotInitialized>(_initd,f,__LINE__);
   assert<NullBufferUsed>(buffer._hostPtr,f,__LINE__);
   cl_event ret;
   cl_int err = clEnqueueWriteBuffer(_id,buffer._id,CL_FALSE,0,
                                     buffer._size*sizeof(T),buffer._hostPtr,0,
                                     NULL,&ret);
   classert<CannotEnqueue>(err,f,__LINE__);
   return CLEvent(ret);
}



}
#endif
