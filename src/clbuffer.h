#ifndef ACCELCOMPENG_CLBUFFER_H
#define ACCELCOMPENG_CLBUFFER_H
#include <CL/cl.h>
#include "exception.h"
namespace AccelCompEng
{



/// @brief OpenCL buffer object.
///
/// Wrapper for a single OpenCL buffer object.
///
/// @tparam T Basic type that this buffer object holds.
template<class T> class CLBuffer
{
public:
   struct CannotCreate : public Exception { using Exception::Exception; };
   struct NullBufferUse : public Exception { using Exception::Exception; };
   struct OutOfRange : public Exception { using Exception::Exception; };
   friend class CLContext;
   friend class CLCommandQueue;
   friend class CLKernel;
   /// @brief Initialize empty object.
   ///
   /// Creates an empty, or null, buffer object.
   CLBuffer() = default;
   ~CLBuffer();
   CLBuffer(const CLBuffer<T>&) = delete;
   CLBuffer& operator=(const CLBuffer<T>&) = delete;
   CLBuffer(CLBuffer<T>&&);
   CLBuffer& operator=(CLBuffer<T>&&);
   T& operator[](int);
   const T& operator[](int) const;
   T& at(int);
   const T& at(int) const;
private:
   CLBuffer(cl_context cid, int size);
   cl_mem _id;
   int _size {0};
   T* _hostPtr {nullptr};
};



template<class T> CLBuffer<T>::~CLBuffer()
{
   if (_hostPtr)
   {
      delete[] _hostPtr;
      clReleaseMemObject(_id);
   }
}



template<class T> CLBuffer<T>::CLBuffer(CLBuffer<T>&& move):
   _id(move._id),
   _size(move._size),
   _hostPtr(move._hostPtr)
{
   move._size = 0;
   move._hostPtr = nullptr;
}



template<class T> CLBuffer<T>& CLBuffer<T>::operator=(CLBuffer<T>&& move)
{
   if (_hostPtr)
   {
      delete[] _hostPtr;
      clReleaseMemObject(_id);
   }
   _id = move._id;
   _size = move._size;
   _hostPtr = move._hostPtr;
   move._size = 0;
   move._hostPtr = nullptr;
}



template<class T> T& CLBuffer<T>::operator[](int i)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullBufferUse>(_hostPtr,f,__LINE__);
   return _hostPtr[i];
}



template<class T> const T& CLBuffer<T>::operator[](int i) const
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullBufferUse>(_hostPtr,f,__LINE__);
   return _hostPtr[i];
}



template<class T> T& CLBuffer<T>::at(int i)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullBufferUse>(_hostPtr,f,__LINE__);
   assert<OutOfRange>(i<_size,f,__LINE__);
   return _hostPtr[i];
}



template<class T> const T& CLBuffer<T>::at(int i) const
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullBufferUse>(_hostPtr,f,__LINE__);
   assert<OutOfRange>(i<_size,f,__LINE__);
   return _hostPtr[i];
}



/// @brief Create buffer object.
///
/// Creates a new OpenCL buffer with the given size.
///
/// @param cid OpenCL context id to use in creation of the buffer object.
/// @param size The size of the buffer array to create.
template<class T> CLBuffer<T>::CLBuffer(cl_context cid, int size):
   _size(size)
{
   static const char* f = __PRETTY_FUNCTION__;
   _hostPtr = new T[size];
   try
   {
      cl_int err;
      _id = clCreateBuffer(cid,CL_MEM_READ_WRITE,size*sizeof(T),_hostPtr,&err);
      classert<CannotCreate>(err,f,__LINE__);
   }
   catch (...)
   {
      delete[] _hostPtr;
      _hostPtr = nullptr;
   }
}



}
#endif
