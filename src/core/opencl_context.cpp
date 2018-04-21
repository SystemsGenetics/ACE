#include "opencl_context.h"
#include "opencl_platform.h"
#include "opencl_device.h"
#include "opencl_common.h"
#include "eexception.h"



using namespace OpenCL;
//






/*!
 * Constructs a new OpenCL context with the given platform, list of devices, and 
 * optional parent. The list of devices must all be part of the same platform or an 
 * exception is thrown. 
 *
 * @param platform Pointer to platform this context is created from. 
 *
 * @param devices List of pointers to devices this context is created from. All 
 *                devices must be part of the given platform. 
 *
 * @param parent Optional parent for this new context. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a new OpenCL context with the given platform and list of devices, 
 *    setting this object's OpenCL context ID to the one returned. If creating the 
 *    context fails then throw an exception. 
 */
Context::Context(Platform* platform, const QList<Device*>& devices, QObject* parent):
   QObject(parent),
   _devices(devices)
{
   cl_int code;
   cl_context_properties properties[] =
   {//
      CL_CONTEXT_PLATFORM,
      (cl_context_properties)platform->id(),
      0
   };
   cl_device_id deviceIds[devices.size()];
   for (int i = 0; i < devices.size() ;++i)
   {
      deviceIds[i] = devices.at(i)->id();
   }
   _id = clCreateContext(properties,devices.size(),deviceIds,nullptr,nullptr,&code);
   if ( code != CL_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      fillException(&e,code);
      throw e;
   }
}






/*!
 * Releases the underlying OpenCL context that this object represents. 
 */
Context::~Context()
{
   clReleaseContext(_id);
}






/*!
 * Returns the OpenCL context ID of this object. 
 *
 * @return OpenCL context ID of this object. 
 */
cl_context Context::id() const
{
   return _id;
}






/*!
 * Returns a read only reference to the list of device pointers which is part of 
 * this context. 
 *
 * @return Read only reference to list of devices of this context. 
 */
const QList<Device*>& Context::devices() const
{
   return _devices;
}
