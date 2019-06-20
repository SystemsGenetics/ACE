#include "opencl_context.h"
#include "opencl_platform.h"
#include "opencl_device.h"
#include "opencl.h"
#include "eexception.h"
#include "edebug.h"



namespace OpenCL
{






/*!
 * Constructs a new OpenCL context with the given list of devices and optional
 * parent. If the given device list is empty or all devices are not part of the
 * same platform then an exception is thrown.
 *
 * @param devices List of pointers to devices this context is created from. All
 *                devices must be part of the given platform.
 *
 * @param parent Optional parent for this new context.
 */
Context::Context(const QList<Device*>& devices, QObject* parent)
   :
   QObject(parent),
   _devices(devices)
{
   // Add the debug header.
   EDEBUG_FUNC(this,&devices,parent);

   // If the given device list is empty then throw an exception, else go to the next
   // step.
   if ( devices.isEmpty() )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Cannot create OpenCL context with empty list of devices."));
      throw e;
   }

   // Create a new OpenCL context with the given platform and list of devices,
   // setting this object's OpenCL context ID to the one returned. If creating the
   // context fails then throw an exception.
   cl_int code;
   cl_context_properties properties[] =
   {
      CL_CONTEXT_PLATFORM,
      reinterpret_cast<cl_context_properties>(devices.first()->platform()),
      0
   };
   QVector<cl_device_id> deviceIds(devices.size());
   for (int i = 0; i < devices.size() ;++i)
   {
      deviceIds[i] = devices.at(i)->id();
   }
   _id = clCreateContext(properties
                         ,static_cast<cl_uint>(devices.size())
                         ,deviceIds.data()
                         ,nullptr
                         ,nullptr
                         ,&code);
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
   EDEBUG_FUNC(this);
   clReleaseContext(_id);
}






/*!
 * Returns the OpenCL context ID of this object.
 *
 * @return OpenCL context ID of this object.
 */
cl_context Context::id() const
{
   EDEBUG_FUNC(this);
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
   EDEBUG_FUNC(this);
   return _devices;
}

}
