#ifndef OPENCLKERNEL_H
#define OPENCLKERNEL_H
#include <CL/cl.h>

#include "openclbuffer.h"
#include "utilities.h"



class OpenCLKernel
{
public:
   OpenCLKernel(cl_program programID, cl_command_queue commandQueueID, cl_device_id deviceID
                , const QString& name);
   ~OpenCLKernel();
   EMAKE_NO_COPY_OR_MOVE(OpenCLKernel);
   template<class T> void setArgument(cl_uint index, T value);
   template<class T> void setBuffer(cl_uint index, OpenCLBuffer<T>* buffer);
   template<class T> void addLocalMemory(cl_uint index, cl_uint size);
   void setDimensionCount(cl_uint count);
   bool setGlobalSize(cl_uint dimension, cl_uint size);
   bool setWorkgroupSize(cl_uint dimension, cl_uint size);
   size_t getMaxWorkgroupSize() const;
   size_t getWorkgroupMultiple() const;
   OpenCLEvent operator()();
private:
   cl_kernel _id;
   cl_command_queue _commandQueueID;
   cl_device_id _deviceID;
   cl_uint _dimensionCount {1};
   size_t* _globalSizes {nullptr};
   size_t* _workgroupSizes {nullptr};
};






template<class T>
void OpenCLKernel::setArgument(cl_uint index, T value)
{
   cl_int code = clSetKernelArg(_id,index,sizeof(T),&value);
   if ( code != CL_SUCCESS )
   {
      OpenCL::throwError("clSetKernelArg",code);
   }
}






template<class T>
void OpenCLKernel::setBuffer(cl_uint index, OpenCLBuffer<T>* buffer)
{
   cl_int code = clSetKernelArg(_id,index,sizeof(cl_mem),buffer->getOpenCLMemory());
   if ( code != CL_SUCCESS )
   {
      OpenCL::throwError("clSetKernelArg",code);
   }
}






template<class T>
void OpenCLKernel::addLocalMemory(cl_uint index, cl_uint size)
{
   size_t byteSize = sizeof(T)*size;
   cl_int code = clSetKernelArg(_id,index,byteSize,nullptr);
   if ( code != CL_SUCCESS )
   {
      OpenCL::throwError("clSetKernelArg",code);
   }
}



#endif
