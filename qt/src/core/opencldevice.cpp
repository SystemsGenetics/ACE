#include "opencldevice.h"
#include "openclprogram.h"
#include "openclkernel.h"
#include "exception.h"



using namespace std;
unique_ptr<EOpenCLDevice> EOpenCLDevice::_instance {nullptr};






EOpenCLDevice::~EOpenCLDevice()
{
   // if command queue exists release it
   if ( _commandQueueID )
   {
      clReleaseCommandQueue(*_commandQueueID);
   }

   // if context exists release it
   if ( _contextID )
   {
      clReleaseContext(*_contextID);
   }
}






EOpenCLDevice& EOpenCLDevice::getInstance()
{
   // create singleton instance first time function is called
   if ( !_instance )
   {
      _instance.reset(new EOpenCLDevice());
   }

   // return reference
   return *_instance;
}






bool EOpenCLDevice::setDevice(cl_platform_id platformID, cl_device_id deviceID)
{
   // make sure state is not bad
   if ( getStatus() == NoOpenCL )
   {
      return false;
   }

   // release command queue and/or context if present
   if ( _commandQueueID )
   {
      clReleaseCommandQueue(*_commandQueueID);
   }
   if ( _contextID )
   {
      clReleaseContext(*_contextID);
   }

   // allocate new memory
   _contextID.reset(new cl_context);
   _commandQueueID.reset(new cl_command_queue);

   // create new context
   _deviceID = deviceID;
   cl_context_properties properties[] =
   {
      CL_CONTEXT_PLATFORM,
      (cl_context_properties)platformID,
      0
   };
   cl_int code;
   *_contextID = clCreateContext(properties,1,&_deviceID,nullptr,nullptr,&code);
   if ( code != CL_SUCCESS )
   {
      _contextID.reset();
      reportError("clCreateContext",code);
      return false;
   }

   // create new command queue
   *_commandQueueID = clCreateCommandQueue(*_contextID,_deviceID
                                           ,CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE,&code);
   if ( code != CL_SUCCESS )
   {
      _commandQueueID.reset();
      reportError("clCreateCommandQueue",code);
      return false;
   }

   // return success
   return true;
}






unique_ptr<EOpenCLProgram> EOpenCLDevice::makeProgram() const
{
   // make sure status is good
   if ( getStatus() != Ok )
   {
      return nullptr;
   }

   // create new program object and return
   return unique_ptr<EOpenCLProgram>(new EOpenCLProgram(_deviceID,*_contextID,*_commandQueueID));
}






quint64 EOpenCLDevice::getGlobalMemorySize() const
{
   // make sure status is good
   if ( getStatus() != Ok )
   {
      return 0;
   }

   // get size information
   unique_ptr<cl_ulong> a(Ace::OpenCL::getDeviceInfo<cl_ulong>(_deviceID
                                                               ,CL_DEVICE_GLOBAL_MEM_SIZE));

   // return size information
   quint64 size = *a;
   return size;
}






quint64 EOpenCLDevice::getLocalMemorySize() const
{
   // make sure status is good
   if ( getStatus() != Ok )
   {
      return 0;
   }

   // get size information
   unique_ptr<cl_ulong> a(Ace::OpenCL::getDeviceInfo<cl_ulong>(_deviceID
                                                               ,CL_DEVICE_LOCAL_MEM_SIZE));

   // return size information
   quint64 size = *a;
   return size;
}






EOpenCLDevice::EOpenCLDevice()
{
   // get very first platform
   cl_uint total;
   cl_int code = clGetPlatformIDs(0,nullptr,&total);
   if ( code != CL_SUCCESS || total == 0 )
   {
      setNoOpenCL();
      return;
   }
   cl_platform_id platformID;
   code = clGetPlatformIDs(1,&platformID,nullptr);
   if ( code != CL_SUCCESS )
   {
      setNoOpenCL();
      return;
   }

   // get very first device of very first platform
   code = clGetDeviceIDs(platformID,CL_DEVICE_TYPE_ALL,0,nullptr,&total);
   if ( code != CL_SUCCESS || total == 0 )
   {
      setNoOpenCL();
      return;
   }
   cl_device_id deviceID;
   code = clGetDeviceIDs(platformID,CL_DEVICE_TYPE_ALL,1,&deviceID,nullptr);
   if ( code != CL_SUCCESS )
   {
      setNoOpenCL();
      return;
   }

   // set device to first one found
   setDevice(platformID,deviceID);
}
