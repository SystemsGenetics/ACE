#include "openclkernel.h"






EOpenCLKernel::EOpenCLKernel(cl_program programID, cl_command_queue commandQueueID
                           , cl_device_id deviceID, const QString &name):
   _id(new cl_kernel),
   _commandQueueID(commandQueueID),
   _deviceID(deviceID),
   _globalSizes(new size_t[1]),
   _workgroupSizes(new size_t[1])
{
   // retain the command queue
   clRetainCommandQueue(commandQueueID);

   // create a new kernel
   cl_int code;
   QByteArray temp = name.toLatin1();
   *_id = clCreateKernel(programID,temp.data(),&code);
   if ( code != CL_SUCCESS )
   {
      _id.reset();
      reportError("clCreateKernel",code);
      return;
   }

   // initialize dimensions, global work size, and workgroup size
   _globalSizes[0] = 1;
   _workgroupSizes[0] = 1;
}






EOpenCLKernel::~EOpenCLKernel()
{
   if ( _id )
   {
      clReleaseKernel(*_id);
   }
   clReleaseCommandQueue(_commandQueueID);
}






void EOpenCLKernel::setDimensionCount(cl_uint count)
{
   if ( count == 0 || getStatus() != Ok )
   {
      return;
   }

   // set new dimension and reallocate size arrays
   _dimensionCount = count;
   _globalSizes.reset(new size_t[count]);
   _workgroupSizes.reset(new size_t[count]);

   // initialize all sizes to 1
   for (unsigned int i = 0; i < count ;++i)
   {
      _globalSizes[i] = 1;
      _workgroupSizes[i] = 1;
   }
}






bool EOpenCLKernel::setGlobalSize(cl_uint dimension, cl_uint size)
{
   // make sure kernel is in ok state and dimension is valid
   if ( dimension >= _dimensionCount || getStatus() != Ok )
   {
      return false;
   }

   // set new global sizes and return true
   _globalSizes[dimension] = size;
   return true;
}






bool EOpenCLKernel::setWorkgroupSize(cl_uint dimension, cl_uint size)
{
   // make sure kernel is in ok state and dimension is valid
   if ( dimension >= _dimensionCount || getStatus() != Ok )
   {
      return false;
   }

   // set new group sizes and return true
   _workgroupSizes[dimension] = size;
   return true;
}






size_t EOpenCLKernel::getMaxWorkgroupSize()
{
   // make sure kernel is in ok state
   if ( getStatus() != Ok )
   {
      return 0;
   }

   // get maximum group size for this kernel
   size_t size;
   cl_int code = clGetKernelWorkGroupInfo(*_id,_deviceID,CL_KERNEL_WORK_GROUP_SIZE,sizeof(size_t)
                                          ,&size,nullptr);
   if ( code != CL_SUCCESS )
   {
      reportError("clGetKernelWorkGroupInfo",code);
      return 0;
   }

   // return maximum group size
   return size;
}






size_t EOpenCLKernel::getWorkgroupMultiple()
{
   // make sure kernel is in ok state
   if ( getStatus() != Ok )
   {
      return 0;
   }

   // get workgroup multiple hint
   size_t size;
   cl_int code = clGetKernelWorkGroupInfo(*_id,_deviceID,CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE
                                          ,sizeof(size_t),&size,nullptr);
   if ( code != CL_SUCCESS )
   {
      reportError("clGetKernelWorkGroupInfo",code);
      return 0;
   }

   // return workgroup multiple hint
   return size;
}






EOpenCLEvent EOpenCLKernel::execute()
{
   // make sure kernel is in ok state
   if ( getStatus() != Ok )
   {
      return EOpenCLEvent();
   }

   // prepare kernel arguments and launch kernel
   size_t offsets[_dimensionCount];
   for (unsigned int i = 0; i < _dimensionCount ;++i)
   {
      offsets[i] = 0;
   }
   cl_event eventID;
   cl_int code = clEnqueueNDRangeKernel(_commandQueueID,*_id,_dimensionCount,offsets
                                        ,_globalSizes.get(),_workgroupSizes.get(),0,nullptr
                                        ,&eventID);
   if ( code != CL_SUCCESS )
   {
      reportError("clEnqueueNDRangeKernel",code);
      return EOpenCLEvent();
   }

   // return OpenCL event of kernel execution
   return EOpenCLEvent(eventID);
}
