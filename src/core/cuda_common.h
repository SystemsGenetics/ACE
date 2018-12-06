#ifndef CUDA_COMMON_H
#define CUDA_COMMON_H
#include <cuda.h>
#include <nvrtc.h>
#include <vector_types.h>
#include "eexception.h"
#include "global.h"



#define CUDA_THROW_ERROR(error)           \
   E_MAKE_EXCEPTION(e);                   \
   e.setTitle(QObject::tr("CUDA Error")); \
   e.setDetails(QObject::tr("%1: %2.")    \
      .arg(getErrorName(error))           \
      .arg(getErrorString(error)));       \
   throw e;



#define CUDA_SAFE_CALL(result)  \
{                               \
   CUresult error = (result);   \
   if ( error != CUDA_SUCCESS ) \
   {                            \
      CUDA_THROW_ERROR(error);  \
   }                            \
}



#define NVRTC_THROW_ERROR(error)          \
   E_MAKE_EXCEPTION(e);                   \
   e.setTitle(QObject::tr("CUDA Error")); \
   e.setDetails(QObject::tr("%1: %2.")    \
      .arg("NVRTC error")                 \
      .arg(getErrorString(error)));       \
   throw e;



#define NVRTC_SAFE_CALL(result)  \
{                                \
   nvrtcResult error = (result); \
   if ( error != NVRTC_SUCCESS ) \
   {                             \
      NVRTC_THROW_ERROR(error);  \
   }                             \
}



/*!
 * This contains all classes and functions associated with using CUDA in ACE.
 */
namespace CUDA
{
   class Device;
   class Context;
   class Program;
   class Stream;
   class Event;
   class Kernel;

   QString getErrorName(CUresult error);
   QString getErrorString(CUresult error);
   QString getErrorString(nvrtcResult result);
}



#endif
