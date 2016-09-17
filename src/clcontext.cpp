#include "clcontext.h"
namespace AccelCompEng
{



CLContext::~CLContext()
{
   if (_initd)
   {
      clReleaseContext(_cid);
   }
}



bool CLContext::is_initd()
{
   return _initd;
}



void CLContext::init_cl(CLDevice& dev)
{
   /// Create new OpenCL context, after which initialize both an OpenCL program and commandqueue.
   /// If any initialization fails, reset object as an empty null object.
   static const char* f = __PRETTY_FUNCTION__;
   cl_context_properties props[] = {
      CL_CONTEXT_PLATFORM, (cl_context_properties)dev.platform(), 0 };
   cl_device_id device {dev.device()};
   cl_int err;
   _cid = clCreateContext(props,1,&device,NULL,NULL,&err);
   classert<CannotCreate>(err,f,__LINE__);
   try
   {
      CLProgram::init(_cid,dev.device());
      CLCommandQueue::init(_cid,dev.device());
   }
   catch (...)
   {
      clReleaseContext(_cid);
      throw;
   }
   _initd = true;
}



}
