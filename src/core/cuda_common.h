#ifndef CUDA_COMMON_H
#define CUDA_COMMON_H
#include <cuda.h>
#include <nvrtc.h>
#include <vector_types.h>
#include "eexception.h"
#include "global.h"
//




namespace CUDA
{
   class Device;
   class Context;
   class Program;
   class Stream;
   class Event;
   class Kernel;

   void throwError(EException* e, CUresult error);
   void throwError(EException* e, nvrtcResult error);
   QString getErrorName(CUresult error);
   QString getErrorString(CUresult error);
   QString getErrorString(nvrtcResult result);
}



#endif
