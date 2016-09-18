#ifndef ACCELCOMPENG_CLKERNEL_H
#define ACCELCOMPENG_CLKERNEL_H
#include <CL/cl.h>
#include "clbuffer.h"
#include "exception.h"
namespace AccelCompEng
{



/// @brief OpenCL kernel.
///
/// Wrapper for an OpenCL kernel.
class CLKernel
{
public:
   struct CannotSetArg : public Exception { using Exception::Exception; };
   struct CannotGetInfo : public Exception { using Exception::Exception; };
   struct TooManyDims : public Exception { using Exception::Exception; };
   struct DimOutOfRange : public Exception { using Exception::Exception; };
   struct NullKernel : public Exception { using Exception::Exception; };
   constexpr static int _maxDims {16};
   friend class CLCommandQueue;
   friend class CLProgram;
   /// @brief Create null kernel.
   ///
   /// Create empty or null object that has no OpenCL kernel.
   CLKernel() = default;
   ~CLKernel();
   CLKernel(const CLKernel&);
   CLKernel& operator=(const CLKernel&);
   CLKernel(CLKernel&&);
   CLKernel& operator=(CLKernel&&);
   /// @brief Set argument.
   ///
   /// Set argument to the kernel.
   ///
   /// @tparam T Value type being passed to kernel argument.
   /// @param index Argument index of the kernel.
   /// @param arg Value to set the kernel to. If the type is size_t it sets the argument as a local
   /// variable. If the type is CLBuffer<T>* it sets the argument as the buffer object given.
   template<class T> void set_arg(cl_uint index, T arg);
   /// @brief Set argument.
   ///
   /// Set argument to the kernel.
   ///
   /// @tparam T Value type being passed to kernel argument.
   /// @param index Argument index of the kernel.
   /// @param arg Value to set the kernel to. If the type is size_t it sets the argument as a local
   /// variable. If the type is CLBuffer<T>* it sets the argument as the buffer object given.
   template<class T> void set_arg(cl_uint index, CLBuffer<T>* arg);
   /// @brief Set all arguments.
   ///
   /// Set all arguments of kernel, much like calling a regular function.
   ///
   /// @tparam Args List of arguments to be passed to the kernel.
   template<class... Args> void set_args(Args...);
   /// @brief Set dimension size.
   ///
   /// Set number of dimensions for the kernel.
   ///
   /// @param dims number of dimensions, must at least be 1.
   void set_swarm_dims(cl_uint dims);
   /// @brief Set global and local size.
   ///
   /// Set global and local size of kernel for a given dimension.
   ///
   /// @param dim The dimension, starting at 0.
   /// @param gSize The new global work size.
   /// @param lSize The new local work size.
   void set_swarm_size(int dim, cl_uint gSize, cl_uint lSize);
   /// @brief Get local workgroup size.
   ///
   /// Get the workgroup size of this kernel for the user selected device.
   ///
   /// @return Largest local work size this kernel can handle.
   size_t get_wg_size();
   /// @brief Get workgroup multiple.
   ///
   /// Get suggested local size multiple of this kernel for the user selected device.
   ///
   /// @return Local work size multiple.
   size_t get_wg_multiple();
private:
   CLKernel(cl_kernel,cl_device_id);
   template<int I,class T> void set_args_int(T);
   template<int I,class T, class... Args> void set_args_int(T,Args...);
   bool _isAlive {false};
   cl_device_id _did;
   cl_kernel _id;
   cl_uint _dims {1};
   size_t _gSizes[_maxDims] {1};
   size_t _lSizes[_maxDims] {1};
};



template<class T> void CLKernel::set_arg(cl_uint index, T arg)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullKernel>(_isAlive,f,__LINE__);
   cl_int err = clSetKernelArg(_id,index,sizeof(T),&arg);
   classert<CannotSetArg>(err,f,__LINE__);
}



template<> inline void CLKernel::set_arg(cl_uint index, size_t lSize)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullKernel>(_isAlive,f,__LINE__);
   cl_int err = clSetKernelArg(_id,index,lSize,NULL);
   classert<CannotSetArg>(err,f,__LINE__);
}



template<class T> void CLKernel::set_arg(cl_uint index, CLBuffer<T>* buffer)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullKernel>(_isAlive,f,__LINE__);
   cl_int err = clSetKernelArg(_id,index,sizeof(cl_mem),&(buffer->_id));
   classert<CannotSetArg>(err,f,__LINE__);
}



template<class... Args> void CLKernel::set_args(Args... args)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullKernel>(_isAlive,f,__LINE__);
   set_args_int<0>(args...);
}



/// @brief Set final argument.
///
/// Set final kernel argument from template list.
///
/// @tparam I Current parameter index.
/// @tparam T Value type of current argument being fetched.
template<int I,class T> void CLKernel::set_args_int(T arg)
{
   set_arg(I,arg);
}



/// @brief Set single argument from list.
///
/// Set kernel argument with next item in argument list, then pass to next.
///
/// @tparam I Current parameter index.
/// @tparam T Value type of current argument being fetched.
/// @tparam Args List of remaining arguments to be passed along.
template<int I,class T, class... Args> void CLKernel::set_args_int(T arg, Args... args)
{
   set_arg(I,arg);
   set_args_int<I+1>(args...);
}



}
#endif
