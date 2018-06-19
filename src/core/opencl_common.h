#ifndef OPENCL_COMMON_H
#define OPENCL_COMMON_H
#include <CL/cl.h>
#include "global.h"
//



namespace OpenCL
{
   void fillException(EException* exception, cl_int code);
}



#endif
