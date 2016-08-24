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
   cl_int err;
   _id = clCreateCommandQueue(cid,did,0x0,&err);
   classert<CannotCreate>(err,__LINE__);
   _initd = true;
}



CLEvent CLCommandQueue::add_task(CLKernel& kernel)
{
   assert<NotInitialized>(_initd,__LINE__);
   assert<NullKernelUsed>(kernel._isAlive,__LINE__);
   cl_event ret;
   cl_int err;
   err = clEnqueueTask(_id,kernel._id,0,NULL,&ret);
   classert<CannotAddTask>(err,__LINE__);
   return CLEvent(ret);
}



CLEvent CLCommandQueue::add_swarm(CLKernel& kernel)
{
   assert<NotInitialized>(_initd,__LINE__);
   assert<NullKernelUsed>(kernel._isAlive,__LINE__);
   cl_event ret;
   const size_t offsets[kernel._dims] = {0};
   cl_int err;
   err = clEnqueueNDRangeKernel(_id,kernel._id,kernel._dims,offsets,
                                kernel._gSizes,kernel._lSizes,
                                0,NULL,&ret);
   classert<CannotAddSwarm>(err,__LINE__);
   return CLEvent(ret);
}



}
