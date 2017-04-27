#include "opencldevice.h"
#include "openclprogram.h"
#include "openclkernel.h"
#include "utilities.h"
#include "exception.h"






OpenCLDevice* OpenCLDevice::_instance = nullptr;






OpenCLDevice::~OpenCLDevice()
{
   // release command queue and/or context if present
   if ( _commandQueueID )
   {
      clReleaseCommandQueue(*_commandQueueID);
   }
   if ( _contextID )
   {
      clReleaseContext(*_contextID);
   }

   // delete allocated memory
   delete _deviceID;
   delete _contextID;
   delete _commandQueueID;
}






OpenCLDevice& OpenCLDevice::getInstance()
{
   if ( !_instance )
   {
      _instance = new OpenCLDevice();
   }
   return *_instance;
}






void OpenCLDevice::initialize()
{
   // get very first platform
   cl_uint total;
   cl_int code = clGetPlatformIDs(0,nullptr,&total);
   if ( code != CL_SUCCESS || total == 0 )
   {
      throwInitializeError();
   }
   cl_platform_id platformID;
   code = clGetPlatformIDs(1,&platformID,nullptr);
   if ( code != CL_SUCCESS )
   {
      throwInitializeError();
   }

   // get very first device of very first platform
   code = clGetDeviceIDs(platformID,CL_DEVICE_TYPE_ALL,0,nullptr,&total);
   if ( code != CL_SUCCESS || total == 0 )
   {
      throwInitializeError();
   }
   cl_device_id deviceID;
   code = clGetDeviceIDs(platformID,CL_DEVICE_TYPE_ALL,1,&deviceID,nullptr);
   if ( code != CL_SUCCESS )
   {
      throwInitializeError();
   }

   // set device to first one found
   setDevice(platformID,deviceID);
}






void OpenCLDevice::setDevice(cl_platform_id platformID, cl_device_id deviceID)
{
   try
   {
      // clear any previously set device and allocate new memory
      clear();
      _deviceID = new cl_device_id;
      _contextID = new cl_context;
      _commandQueueID = new cl_command_queue;

      // create new context
      *_deviceID = deviceID;
      cl_context_properties properties[] =
      {
         CL_CONTEXT_PLATFORM,
         (cl_context_properties)platformID,
         0
      };
      cl_int code;
      *_contextID = clCreateContext(properties,1,_deviceID,nullptr,nullptr,&code);
      if ( code != CL_SUCCESS )
      {
         OpenCL::throwError("clCreateContext",code);
      }

      // create new command queue
      *_commandQueueID = clCreateCommandQueue(*_contextID,*_deviceID
                                              ,CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE,&code);
      if ( code != CL_SUCCESS )
      {
         OpenCL::throwError("clCreateCommandQueue",code);
      }
   }
   catch (...)
   {
      clear();
      throw;
   }
}






void OpenCLDevice::clear()
{
   // release command queue and/or context if present
   if ( _commandQueueID )
   {
      clReleaseCommandQueue(*_commandQueueID);
   }
   if ( _contextID )
   {
      clReleaseContext(*_contextID);
   }

   // delete allocated memory and set pointers to null
   delete _deviceID;
   delete _contextID;
   delete _commandQueueID;
   _deviceID = nullptr;
   _contextID = nullptr;
   _commandQueueID = nullptr;
}






unique_ptr<OpenCLProgram> OpenCLDevice::makeProgram() const
{
   if ( !_deviceID || !_contextID || !_commandQueueID )
   {
      return nullptr;
   }
   return unique_ptr<OpenCLProgram>(new OpenCLProgram(*_deviceID,*_contextID,*_commandQueueID));
}






quint64 OpenCLDevice::getGlobalMemorySize() const
{
   if ( !_deviceID )
   {
      return 0;
   }
   unique_ptr<cl_ulong> a(OpenCL::getDeviceInfo<cl_ulong>(*_deviceID,CL_DEVICE_GLOBAL_MEM_SIZE));
   quint64 size = *a;
   return size;
}






quint64 OpenCLDevice::getLocalMemorySize() const
{
   if ( !_deviceID )
   {
      return 0;
   }
   unique_ptr<cl_ulong> a(OpenCL::getDeviceInfo<cl_ulong>(*_deviceID,CL_DEVICE_LOCAL_MEM_SIZE));
   quint64 size = *a;
   return size;
}






void OpenCLDevice::throwInitializeError()
{
   MAKE_EXCEPTION(e);
   e.setTitle(QObject::tr("Cannot Initialize OpenCL"));
   e.out() << QObject::tr("Cannot query OpenCL platforms or devices. It appears your system does"
                          " not have OpenCL installed on it.");
   throw e;
}
