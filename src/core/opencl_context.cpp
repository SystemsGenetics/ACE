#include "opencl_context.h"
#include "opencl_platform.h"
#include "opencl_device.h"
#include "opencl_common.h"
#include "eexception.h"



using namespace OpenCL;
//






/*!
 *
 * @param platform  
 *
 * @param devices  
 *
 * @param parent  
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
 */
Context::~Context()
{
   clReleaseContext(_id);
}






/*!
 */
cl_context Context::id() const
{
   return _id;
}






/*!
 */
const QList<Device*>& Context::devices() const
{
   return _devices;
}
