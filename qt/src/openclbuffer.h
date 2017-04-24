#ifndef OPENCLBUFFER_H
#define OPENCLBUFFER_H
#include <CL/cl.h>
#include <QtCore>

#include "openclevent.h"
#include "utilities.h"



template<class T>
class OpenCLBuffer
{
public:
   OpenCLBuffer(cl_context contextID, cl_command_queue commandQueueID, quint64 size);
   ~OpenCLBuffer();
   OpenCLBuffer(const OpenCLBuffer<T>&) = delete;
   OpenCLBuffer& operator=(const OpenCLBuffer<T>&) = delete;
   OpenCLBuffer(OpenCLBuffer<T>&&) = delete;
   OpenCLBuffer& operator=(OpenCLBuffer<T>&&) = delete;
   T& operator[](int index);
   const T& operator[](int index) const;
   OpenCLEvent read();
   OpenCLEvent write();
   cl_mem* getOpenCLMemory();
private:
   cl_command_queue _commandQueueID;
   cl_mem _id;
   int _size;
   T* _host {nullptr};
};






template<class T>
OpenCLBuffer<T>::OpenCLBuffer(cl_context contextID, cl_command_queue commandQueueID, quint64 size):
   _commandQueueID(commandQueueID),
   _size(size)
{
   try
   {
      // retain the command queue and allocate host memory
      clRetainCommandQueue(commandQueueID);
      _host = new T[size];

      // create new buffer object
      cl_int code;
      _id = clCreateBuffer(contextID,CL_MEM_READ_WRITE,size*sizeof(T),_host,&code);
      if ( code != CL_SUCCESS )
      {
         OpenCL::throwError("clCreateBuffer",code);
      }
   }
   catch (...)
   {
      // if anything goes wrong delete host memory
      delete[] _host;
      throw;
   }
}






template<class T>
OpenCLBuffer<T>::~OpenCLBuffer()
{
   clReleaseMemObject(_id);
   clReleaseCommandQueue(_commandQueueID);
   delete[] _host;
}






template<class T>
T& OpenCLBuffer<T>::operator[](int index)
{
   return _host[index];
}






template<class T>
const T& OpenCLBuffer<T>::operator[](int i) const
{
   return _host[index];
}






template<class T>
OpenCLEvent OpenCLBuffer<T>::read()
{
   cl_event eventID;
   cl_int code = clEnqueueReadBuffer(_commandQueueID,_id,CL_FALSE,0,_size*sizeof(T),_host,0,nullptr
                                     ,&eventID);
   if ( code != CL_SUCCESS )
   {
      OpenCL::throwError("clEnqueueReadBuffer",code);
   }
   return OpenCLEvent(eventID);
}






template<class T>
OpenCLEvent OpenCLBuffer<T>::write()
{
   cl_event eventID;
   cl_int code = clEnqueueWriteBuffer(_commandQueueID,_id,CL_FALSE,0,_size*sizeof(T),_host,0,nullptr
                                      ,&eventID);
   if ( code != CL_SUCCESS )
   {
      OpenCL::throwError("clEnqueueWriteBuffer",code);
   }
   return OpenCLEvent(eventID);
}






template<class T>
cl_mem* OpenCLBuffer<T>::getOpenCLMemory()
{
   return &_id;
}



#endif
