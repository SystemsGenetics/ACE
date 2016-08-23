#ifndef ACCELCOMPENG_CLCOMMANDQUEUE_H
#define ACCELCOMPENG_CLCOMMANDQUEUE_H
#include <CL/cl.h>
#include "clkernel.h"
#include "clbuffer.h"
#include "clevent.h"
namespace AccelCompEng
{



class CLCommandQueue
{
public:
   ACE_EXCEPTION(AccelCompEng::CLCommandQueue,CannotCreate)
   ACE_EXCEPTION(AccelCompEng::CLCommandQueue,CannotEnqueueRB)
   ACE_EXCEPTION(AccelCompEng::CLCommandQueue,CannotEnqueueWB)
   ACE_EXCEPTION(AccelCompEng::CLCommandQueue,CannotAddTask)
   ACE_EXCEPTION(AccelCompEng::CLCommandQueue,CannotAddSwarm)
   ACE_EXCEPTION(AccelCompEng::CLCommandQueue,NotInitialized)
   ACE_EXCEPTION(AccelCompEng::CLCommandQueue,NullBufferUsed)
   ACE_EXCEPTION(AccelCompEng::CLCommandQueue,NullKernelUsed)
   CLCommandQueue() = default;
   ~CLCommandQueue();
   CLCommandQueue(const CLCommandQueue&) = delete;
   CLCommandQueue& operator=(const CLCommandQueue&) = delete;
   CLCommandQueue(CLCommandQueue&&) = delete;
   CLCommandQueue& operator=(CLCommandQueue&&) = delete;
protected:
   void init(cl_context,cl_device_id);
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
   static const char* notInit = "OpenCL command queue not initialized";
   static const char* nullBuf = "Cannot use OpenCL buffer that is null";
   static const char* cannotEnqueue = "Failed adding OpenCL read buffer command";
   assert<NotInitialized>(_initd,__LINE__,notInit);
   assert<NullBufferUsed>(buffer._hostPtr,__LINE__,nullBuf);
   cl_event ret;
   cl_int err = clEnqueueReadBuffer(_id,buffer._id,CL_FALSE,0,
                                    buffer._size*sizeof(T),buffer._hostPtr,0,
                                    NULL,&ret);
   classert<CannotEnqueueRB>(err,__LINE__,cannotEnqueue);
   return CLEvent(ret);
}



template<class T> CLEvent CLCommandQueue::write_buffer(CLBuffer<T>& buffer)
{
   static const char* notInit = "OpenCL command queue not initialized";
   static const char* nullBuf = "Cannot use OpenCL buffer that is null";
   static const char* failEnqueue = "Failed adding OpenCL write buffer command";
   assert<NotInitialized>(_initd,__LINE__,notInit);
   assert<NullBufferUsed>(buffer._hostPtr,__LINE__,nullBuf);
   cl_event ret;
   cl_int err = clEnqueueWriteBuffer(_id,buffer._id,CL_FALSE,0,
                                     buffer._size*sizeof(T),buffer._hostPtr,0,
                                     NULL,&ret);
   classert<CannotEnqueueWB>(err,__LINE__,failEnqueue);
   return CLEvent(ret);
}



}
#endif
