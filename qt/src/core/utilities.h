#ifndef UTILITIES_H
#define UTILITIES_H
#include <CL/cl.h>
#include <QtCore>
#include <memory>
#define ACE_DISBALE_COPY_AND_MOVE(name) name(const name&) = delete;\
   name& operator=(const name&) = delete;name(name&&) = delete;name& operator=(name&&) = delete;
#define ACE_DISBALE2_COPY_AND_MOVE(first,second) first(const second&) = delete;\
   first& operator=(const second&) = delete;first(second&&) = delete;\
   first& operator=(second&&) = delete;



namespace Ace
{
   QString formatMemorySize(quint64 size);
}



namespace Ace
{
   namespace OpenCL
   {
      namespace ReturnCodes
      {
         enum
         {
            Success = 0
            ,DeviceNotFound = -1
            ,DeviceNotAvailable = -2
            ,CompilerNotAvailable = -3
            ,MemObjectAllocationFailure = -4
            ,OutOfResources = -5
            ,OutOfHostMemory = -6
            ,ProfilingInfoNotAvailable = -7
            ,MemCopyOverlap = -8
            ,ImageFormatMismatch = -9
            ,ImageFormatNotSupported = -10
            ,BuildProgramFailure = -11
            ,MapFailure = -12
            ,InvalidValue = -30
            ,InvalidDeviceType = -31
            ,InvalidPlatform = -32
            ,InvalidDevice = -33
            ,InvalidContext = -34
            ,InvalidQueueProperties = -35
            ,InvalidCommandQueue = -36
            ,InvalidHostPtr = -37
            ,InvalidMemObject = -38
            ,InvalidImageFormatDescriptor = -39
            ,InvalidImageSize = -40
            ,InvalidSampler = -41
            ,InvalidBinary = -42
            ,InvalidBuildOptions = -43
            ,InvalidProgram = -44
            ,InvalidProgramExecutable = -45
            ,InvalidKernelName = -46
            ,InvalidKernelDefinition = -47
            ,InvalidKernel = -48
            ,InvalidArgIndex = -49
            ,InvalidArgValue = -50
            ,InvalidArgSize = -51
            ,InvalidKernelArgs = -52
            ,InvalidWorkDimension = -53
            ,InvalidWorkGroupSize = -54
            ,InvalidWorkItemSize = -55
            ,InvalidGlobalOffset = -56
            ,InvalidEventWaitList = -57
            ,InvalidEvent = -58
            ,InvalidOperation = -59
            ,InvalidGLObject = -60
            ,InvalidBufferSize = -61
            ,InvalidMipLevel = -62
            ,InvalidGlobalWorkSize = -63
            ,InvalidProperty = -64
         };
      }
      QString generateErrorString(cl_int code);
      void throwError(const QString& function, cl_int code);
      template<class T> std::unique_ptr<T> getDeviceInfo(cl_device_id id, cl_device_info what);
   }
}






template<class T>
std::unique_ptr<T> Ace::OpenCL::getDeviceInfo(cl_device_id id, cl_device_info what)
{
   // determine the size of the variable requested in bytes
   size_t size;
   cl_int code = clGetDeviceInfo(id,what,0,nullptr,&size);
   if ( code != CL_SUCCESS )
   {
      throwError("clGetDeviceInfo",code);
   }

   // allocate memory needed and get information data
   T* info = reinterpret_cast<T*>(new char[size]);
   code = clGetDeviceInfo(id,what,size,info,nullptr);
   if ( code != CL_SUCCESS )
   {
      delete[] reinterpret_cast<char*>(info);
      throwError("clGetDeviceInfo",code);
   }
   return std::unique_ptr<T>(info);
}



#endif
