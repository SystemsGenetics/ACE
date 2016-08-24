#ifndef ACCELCOMPENG_CLBUFFER_H
#define ACCELCOMPENG_CLBUFFER_H
#include <CL/cl.h>
#include "exception.h"
namespace AccelCompEng
{



template<class T> class CLBuffer
{
public:
   struct CannotCreate : public Exception { using Exception::Exception; };
   struct NullBufferUse : public Exception { using Exception::Exception; };
   struct OutOfRange : public Exception { using Exception::Exception; };
   friend class CLContext;
   friend class CLCommandQueue;
   friend class CLKernel;
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
   CLBuffer(cl_context,int);
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
   assert<NullBufferUse>(_hostPtr,__LINE__);
   return _hostPtr[i];
}



template<class T> const T& CLBuffer<T>::operator[](int i) const
{
   assert<NullBufferUse>(_hostPtr,__LINE__);
   return _hostPtr[i];
}



template<class T> T& CLBuffer<T>::at(int i)
{
   assert<NullBufferUse>(_hostPtr,__LINE__);
   assert<OutOfRange>(i<_size,__LINE__);
   return _hostPtr[i];
}



template<class T> const T& CLBuffer<T>::at(int i) const
{
   assert<NullBufferUse>(_hostPtr,__LINE__);
   assert<OutOfRange>(i<_size,__LINE__);
   return _hostPtr[i];
}



template<class T> CLBuffer<T>::CLBuffer(cl_context cid, int size):
   _size(size)
{
   _hostPtr = new T[size];
   try
   {
      cl_int err;
      _id = clCreateBuffer(cid,CL_MEM_READ_WRITE,size*sizeof(T),_hostPtr,&err);
      classert<CannotCreate>(err==CL_SUCCESS,__LINE__);
   }
   catch (...)
   {
      delete[] _hostPtr;
      _hostPtr = nullptr;
   }
}



}
#endif
