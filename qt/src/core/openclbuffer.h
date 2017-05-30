#ifndef EOPENCLBUFFER_H
#define EOPENCLBUFFER_H
#include <CL/cl.h>
#include <QtCore>
#include <memory>

#include "openclevent.h"
#include "opencl.h"



template<class T>
class EOpenCLBuffer : public EOpenCL
{
public:
   EOpenCLBuffer(cl_context contextID, cl_command_queue commandQueueID, quint64 size);
   ~EOpenCLBuffer();
   ACE_DISBALE2_COPY_AND_MOVE(EOpenCLBuffer,EOpenCLBuffer<T>)
   T& operator[](int index);
   const T& operator[](int index) const;
   EOpenCLEvent read();
   EOpenCLEvent write();
   cl_mem* getOpenCLMemory();
private:
   cl_command_queue _commandQueueID;
   std::unique_ptr<cl_mem> _id {nullptr};
   int _size {0};
   std::unique_ptr<T[]> _host {nullptr};
};






template<class T>
EOpenCLBuffer<T>::EOpenCLBuffer(cl_context contextID, cl_command_queue commandQueueID
                                , quint64 size):
   _commandQueueID(commandQueueID),
   _size(size),
   _id(new cl_mem),
   _host(new T[size])
{
   // retain the command queue
   clRetainCommandQueue(commandQueueID);

   // create new buffer object
   cl_int code;
   *_id = clCreateBuffer(contextID,CL_MEM_READ_WRITE,size*sizeof(T),_host,&code);
   if ( code != CL_SUCCESS )
   {
      _id.reset();
      reportError("clCreateBuffer",code);
      return;
   }
}






template<class T>
EOpenCLBuffer<T>::~EOpenCLBuffer()
{
   if ( _id )
   {
      clReleaseMemObject(*_id);
   }
   clReleaseCommandQueue(_commandQueueID);
}






template<class T>
T& EOpenCLBuffer<T>::operator[](int index)
{
   return _host[index];
}






template<class T>
const T& EOpenCLBuffer<T>::operator[](int i) const
{
   return _host[index];
}






template<class T>
EOpenCLEvent EOpenCLBuffer<T>::read()
{
   if ( getStatus() != Ok )
   {
      return EOpenCLEvent();
   }
   cl_event eventID;
   cl_int code = clEnqueueReadBuffer(_commandQueueID,*_id,CL_FALSE,0,_size*sizeof(T),_host,0,nullptr
                                     ,&eventID);
   if ( code != CL_SUCCESS )
   {
      reportError("clEnqueueReadBuffer",code);
      return EOpenCLEvent();
   }
   return EOpenCLEvent(eventID);
}






template<class T>
EOpenCLEvent EOpenCLBuffer<T>::write()
{
   if ( getStatus() != Ok )
   {
      return EOpenCLEvent();
   }
   cl_event eventID;
   cl_int code = clEnqueueWriteBuffer(_commandQueueID,*_id,CL_FALSE,0,_size*sizeof(T),_host,0
                                      ,nullptr,&eventID);
   if ( code != CL_SUCCESS )
   {
      reportError("clEnqueueWriteBuffer",code);
      return EOpenCLEvent();
   }
   return EOpenCLEvent(eventID);
}






template<class T>
cl_mem* EOpenCLBuffer<T>::getOpenCLMemory()
{
   return &_id;
}






#endif
