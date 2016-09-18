#ifndef ACCELCOMPENG_CLCOMMANDQUEUE_H
#define ACCELCOMPENG_CLCOMMANDQUEUE_H
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/cl.h>
#include "clkernel.h"
#include "clbuffer.h"
#include "clevent.h"
namespace AccelCompEng
{



/// @brief Wrapper for OpenCL command queue.
///
/// Wrapper for OpenCL command queue connected to device selected by user. This command queue is
/// set to execute commands out of order, so order of execution without using events is not
/// deterministic.
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
   /// @brief Create empty object.
   ///
   /// Creates an empty wrapper that has no command queue setup yet.
   CLCommandQueue() = default;
   ~CLCommandQueue();
   CLCommandQueue(const CLCommandQueue&) = delete;
   CLCommandQueue& operator=(const CLCommandQueue&) = delete;
   CLCommandQueue(CLCommandQueue&&) = delete;
   CLCommandQueue& operator=(CLCommandQueue&&) = delete;
   /// @brief Initialize new command queue.
   ///
   /// Initializes the wrapper with a new OpenCL command queue using the given context and devicce.
   ///
   /// @param cid OpenCL context id used to create the command queue.
   /// @param did OpenCL device id used to create the command queue.
   void init(cl_context cid, cl_device_id did);
protected:
   /// @brief Read buffer.
   ///
   /// Adds a read buffer command to the queue.
   ///
   /// @param buffer Object to read from.
   template<class T> CLEvent read_buffer(CLBuffer<T>& buffer);
   /// @brief Write buffer.
   ///
   /// Adds a write buffer command to the queue.
   ///
   /// @param buffer Object to write to.
   template<class T> CLEvent write_buffer(CLBuffer<T>& buffer);
   /// @brief Add single task.
   ///
   /// Adds single task to the queue.
   ///
   /// @param kernel Object to execute on device.
   CLEvent add_task(CLKernel& kernel);
   /// @brief Add parallel swarm.
   ///
   /// Adds parallel range of tasks to the queue.
   ///
   /// @param kernel Object to parallel execute on device.
   CLEvent add_swarm(CLKernel& kernel);
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
