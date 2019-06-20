#ifndef OPENCL_H
#define OPENCL_H
#include <CL/cl.h>
#include "global.h"



/*!
 * This contains all classes and functions associated with using OpenCL in ACE.
 */
namespace OpenCL
{
   class Platform;
   class Device;
   class Context;
   class Program;
   class CommandQueue;
   class Event;
   class Kernel;
   void fillException(EException* exception, cl_int code);
}

#endif
