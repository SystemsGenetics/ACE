#ifndef OPENCLKERNEL_H
#define OPENCLKERNEL_H
#include <CL/cl.h>
#include <memory>

#include "openclbuffer.h"
#include "opencl.h"



class EOpenCLKernel : public EOpenCL
{
public:
   EOpenCLKernel(cl_program programID, cl_command_queue commandQueueID, cl_device_id deviceID
                , const QString& name);
   ~EOpenCLKernel();
   ACE_DISBALE_COPY_AND_MOVE(EOpenCLKernel)
   template<class T> void setArgument(cl_uint index, T value);
   template<class T> void setBuffer(cl_uint index, EOpenCLBuffer<T>* buffer);
   template<class T> void addLocalMemory(cl_uint index, cl_uint size);
   void setDimensionCount(cl_uint count);
   bool setGlobalSize(cl_uint dimension, cl_uint size);
   bool setWorkgroupSize(cl_uint dimension, cl_uint size);
   size_t getMaxWorkgroupSize();
   size_t getWorkgroupMultiple();
   EOpenCLEvent execute();
private:
   std::unique_ptr<cl_kernel> _id {nullptr};
   cl_command_queue _commandQueueID {0};
   cl_device_id _deviceID {0};
   cl_uint _dimensionCount {1};
   std::unique_ptr<size_t[]> _globalSizes {nullptr};
   std::unique_ptr<size_t[]> _workgroupSizes {nullptr};
};






template<class T>
void EOpenCLKernel::setArgument(cl_uint index, T value)
{
   // make sure kernel is in ok state
   if ( getStatus() != Ok )
   {
      return;
   }

   // set kernel argument
   cl_int code = clSetKernelArg(*_id,index,sizeof(T),&value);
   if ( code != CL_SUCCESS )
   {
      Ace::OpenCL::throwError("clSetKernelArg",code);
   }
}






template<class T>
void EOpenCLKernel::setBuffer(cl_uint index, EOpenCLBuffer<T>* buffer)
{
   // make sure kernel is in ok state
   if ( getStatus() != Ok )
   {
      return;
   }

   // set kernel argument to buffer
   cl_int code = clSetKernelArg(*_id,index,sizeof(cl_mem),buffer->getOpenCLMemory());
   if ( code != CL_SUCCESS )
   {
      Ace::OpenCL::throwError("clSetKernelArg",code);
   }
}






template<class T>
void EOpenCLKernel::addLocalMemory(cl_uint index, cl_uint size)
{
   // make sure kernel is in ok state
   if ( getStatus() != Ok )
   {
      return;
   }

   // set kernel argument for allocated local memory
   size_t byteSize = sizeof(T)*size;
   cl_int code = clSetKernelArg(*_id,index,byteSize,nullptr);
   if ( code != CL_SUCCESS )
   {
      Ace::OpenCL::throwError("clSetKernelArg",code);
   }
}



#endif
