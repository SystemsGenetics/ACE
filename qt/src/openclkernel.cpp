#include "openclkernel.h"






OpenCLKernel::OpenCLKernel(cl_program programID, cl_command_queue commandQueueID
                           , cl_device_id deviceID, const QString &name):
   _commandQueueID(commandQueueID),
   _deviceID(deviceID)
{
   try
   {
      // create a new kernel
      cl_int code;
      QByteArray temp = name.toLatin1();
      _id = clCreateKernel(programID,temp.data(),&code);
      if ( code != CL_SUCCESS )
      {
         OpenCL::throwError("clCreateKernel",code);
      }

      // initialize dimensions, global work size, and workgroup size
      _globalSizes = new size_t[1];
      _workgroupSizes = new size_t[1];
      _globalSizes[0] = 1;
      _workgroupSizes[0] = 1;
   }
   catch (...)
   {
      delete[] _globalSizes;
      delete[] _workgroupSizes;
      throw;
   }
}






OpenCLKernel::~OpenCLKernel()
{
   clReleaseKernel(_id);
   clReleaseCommandQueue(_commandQueueID);
   delete[] _globalSizes;
   delete[] _workgroupSizes;
}






void OpenCLKernel::setDimensionCount(cl_uint count)
{
   // set new dimension and reallocate size arrays
   _dimensionCount = count;
   delete[] _globalSizes;
   delete[] _workgroupSizes;
   _globalSizes = new size_t[count];
   _workgroupSizes = new size_t[count];

   // initialize all sizes to 1
   for (unsigned int i = 0; i < count ;++i)
   {
      _globalSizes[i] = 1;
      _workgroupSizes[i] = 1;
   }
}






bool OpenCLKernel::setGlobalSize(cl_uint dimension, cl_uint size)
{
   if ( dimension >= _dimensionCount )
   {
      return false;
   }
   _globalSizes[dimension] = size;
   return true;
}






bool OpenCLKernel::setWorkgroupSize(cl_uint dimension, cl_uint size)
{
   if ( dimension >= _dimensionCount )
   {
      return false;
   }
   _workgroupSizes[dimension] = size;
   return true;
}






size_t OpenCLKernel::getMaxWorkgroupSize() const
{
   size_t size;
   cl_int code = clGetKernelWorkGroupInfo(_id,_deviceID,CL_KERNEL_WORK_GROUP_SIZE,sizeof(size_t)
                                          ,&size,nullptr);
   if ( code != CL_SUCCESS )
   {
      OpenCL::throwError("clGetKernelWorkGroupInfo",code);
   }
   return size;
}






size_t OpenCLKernel::getWorkgroupMultiple() const
{
   size_t size;
   cl_int code = clGetKernelWorkGroupInfo(_id,_deviceID,CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE
                                          ,sizeof(size_t),&size,nullptr);
   if ( code != CL_SUCCESS )
   {
      OpenCL::throwError("clGetKernelWorkGroupInfo",code);
   }
   return size;
}






OpenCLEvent OpenCLKernel::operator()()
{
   size_t offsets[_dimensionCount];
   for (unsigned int i = 0; i < _dimensionCount ;++i)
   {
      offsets[i] = 0;
   }
   cl_event eventID;
   cl_int code = clEnqueueNDRangeKernel(_commandQueueID,_id,_dimensionCount,offsets,_globalSizes
                                        ,_workgroupSizes,0,nullptr,&eventID);
   if ( code != CL_SUCCESS )
   {
      OpenCL::throwError("clEnqueueNDRangeKernel",code);
   }
   return OpenCLEvent(eventID);
}
