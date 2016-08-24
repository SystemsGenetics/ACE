#include "clcommandqueue.h"
namespace AccelCompEng
{



CLCommandQueue::~CLCommandQueue()
{
   if (_initd)
   {
      clReleaseCommandQueue(_id);
   }
}



void CLCommandQueue::init(cl_context cid, cl_device_id did)
{
   static const char* f = __PRETTY_FUNCTION__;
   cl_int err;
   _id = clCreateCommandQueue(cid,did,0x0,&err);
   classert<CannotCreate>(err,f,__LINE__);
   _initd = true;
}



CLEvent CLCommandQueue::add_task(CLKernel& kernel)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NotInitialized>(_initd,f,__LINE__);
   assert<NullKernelUsed>(kernel._isAlive,f,__LINE__);
   cl_event ret;
   cl_int err;
   err = clEnqueueTask(_id,kernel._id,0,NULL,&ret);
   classert<CannotAddTask>(err,f,__LINE__);
   return CLEvent(ret);
}



CLEvent CLCommandQueue::add_swarm(CLKernel& kernel)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NotInitialized>(_initd,f,__LINE__);
   assert<NullKernelUsed>(kernel._isAlive,f,__LINE__);
   cl_event ret;
   const size_t offsets[kernel._dims] = {0};
   cl_int err;
   err = clEnqueueNDRangeKernel(_id,kernel._id,kernel._dims,offsets,
                                kernel._gSizes,kernel._lSizes,
                                0,NULL,&ret);
   classert<CannotAddSwarm>(err,f,__LINE__);
   return CLEvent(ret);
}



}
