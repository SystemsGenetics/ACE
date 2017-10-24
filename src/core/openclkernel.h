#ifndef OPENCLKERNEL_H
#define OPENCLKERNEL_H
#include <CL/cl.h>
#include <memory>

#include "openclbuffer.h"
#include "opencl.h"



/// Holds a single kernel that can be used for execution on OpenCL device.
class EOpenCLKernel : public EOpenCL
{
public:
   /// Internal command DO NOT USE.
   EOpenCLKernel(cl_program programID, cl_command_queue commandQueueID, cl_device_id deviceID
                , const QString& name);
   ~EOpenCLKernel();
   EOpenCLKernel(const EOpenCLKernel&) = delete;
   /// Set kernel private argument.
   ///
   /// @param index Argument index.
   /// @param value Argument value.
   template<class T> void setArgument(cl_uint index, T value);
   /// Set kernel buffer argument.
   ///
   /// @param index Argument index.
   /// @param buffer Pointer to buffer object.
   template<class T> void setBuffer(cl_uint index, EOpenCLBuffer<T>* buffer);
   /// Allocate local memory to kernel argument.
   ///
   /// @param index Argument index.
   /// @param size Size of local memory in bytes.
   template<class T> void addLocalMemory(cl_uint index, cl_uint size);
   /// Set number of dimenions for kernel.
   ///
   /// @param count Number of dimenions.
   void setDimensionCount(cl_uint count);
   /// Set global size of kernel for given dimension.
   ///
   /// @param dimension Dimension index.
   /// @param size Number of kernels.
   ///
   /// @return True if successful.
   bool setGlobalSize(cl_uint dimension, cl_uint size);
   /// Set workgroup size of kernel for given dimension.
   ///
   /// @param dimension Dimension index.
   /// @param size Number of kernels per workgroup.
   ///
   /// @return True if successful.
   bool setWorkgroupSize(cl_uint dimension, cl_uint size);
   /// Get maximum workgroup size for kernel object.
   ///
   /// @return Maximum workgroup size.
   size_t getMaxWorkgroupSize();
   /// Get recommended workgroup multiple for kernel object.
   ///
   /// @return Recommended workgroup multiple.
   size_t getWorkgroupMultiple();
   /// Execute kernel on OpenCL device.
   ///
   /// @return OpenCL event for kernel execution.
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
