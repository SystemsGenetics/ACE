#include "opencl_device.h"
#include "opencl.h"



namespace OpenCL
{






/*!
 * Constructs a new OpenCL device object with the given OpenCL device ID and
 * optional parent.
 *
 * @param id OpenCL device ID used to construct this new device object.
 *
 * @param parent Optional parent for this new object.
 */
Device::Device(cl_device_id id, QObject* parent)
   :
   QObject(parent),
   _id(id),
   _available(getInfo<bool,cl_bool>(CL_DEVICE_AVAILABLE)),
   _compiler(getInfo<bool,cl_bool>(CL_DEVICE_COMPILER_AVAILABLE)),
   _extensions(getStringInfo(CL_DEVICE_EXTENSIONS).split(' ')),
   _globalMemorySize(getInfo<qint64,cl_ulong>(CL_DEVICE_GLOBAL_MEM_SIZE)),
   _localMemorySize(getInfo<qint64,cl_ulong>(CL_DEVICE_LOCAL_MEM_SIZE)),
   _computeUnitSize(getInfo<int,cl_uint>(CL_DEVICE_MAX_COMPUTE_UNITS)),
   _clockFrequency(getInfo<int,cl_uint>(CL_DEVICE_MAX_CLOCK_FREQUENCY)),
   _name(getStringInfo(CL_DEVICE_NAME)),
   _platform(getInfo<cl_platform_id,cl_platform_id>(CL_DEVICE_PLATFORM))
{
   // Add the debug header.
   EDEBUG_FUNC(this,static_cast<void*>(id),parent);

   // Initialize all data and query the device type, setting this object's device
   // type to the type returned.
   switch (getInfo<cl_device_type,cl_device_type>(CL_DEVICE_TYPE))
   {
   case CL_DEVICE_TYPE_CPU:
      _type = Type::CPU;
      break;
   case CL_DEVICE_TYPE_GPU:
      _type = Type::GPU;
      break;
   case CL_DEVICE_TYPE_ACCELERATOR:
      _type = Type::Accelerator;
      break;
   case CL_DEVICE_TYPE_DEFAULT:
      _type = Type::Default;
      break;
   }
}






/*!
 * Returns the OpenCL device ID for this device object.
 *
 * @return OpenCL device ID for this object.
 */
cl_device_id Device::id() const
{
   EDEBUG_FUNC(this);
   return _id;
}






/*!
 * Indicates if this device object is available for use with OpenCL.
 *
 * @return True if this device is available or false otherwise.
 */
bool Device::isAvailable() const
{
   EDEBUG_FUNC(this);
   return _available;
}






/*!
 * Indicates if this device has the ability to compile OpenCL kernel code.
 *
 * @return True if this device can compile kernel code or false otherwise.
 */
bool Device::hasCompiler() const
{
   EDEBUG_FUNC(this);
   return _compiler;
}






/*!
 * Returns the OpenCL extensions supported by this device.
 *
 * @return OpenCL extensions supported by this device.
 */
const QStringList& Device::extensions() const
{
   EDEBUG_FUNC(this);
   return _extensions;
}






/*!
 * Returns the global memory size of this device in bytes.
 *
 * @return Global memory size of this device in bytes.
 */
qint64 Device::globalMemorySize() const
{
   EDEBUG_FUNC(this);
   return _globalMemorySize;
}






/*!
 * Returns the local memory size of this device in bytes.
 *
 * @return Local memory size of this device in bytes.
 */
qint64 Device::localMemorySize() const
{
   EDEBUG_FUNC(this);
   return _localMemorySize;
}






/*!
 * Returns the total number of compute units for this device's underlying
 * hardware. In other words how many compute units can operate at any one time
 * in parallel.
 *
 * @return Total number of compute units for this device's hardware.
 */
int Device::computeUnitSize() const
{
   EDEBUG_FUNC(this);
   return _computeUnitSize;
}






/*!
 * Returns the maximum clock frequency for this device's hardware in megahertz.
 * Usually in all modern GPU devices it can run at many different clock speeds.
 *
 * @return Maximum clock frequency for this device's hardware in megahertz.
 */
int Device::clockFrequency() const
{
   EDEBUG_FUNC(this);
   return _clockFrequency;
}






/*!
 * Returns the name of this device.
 *
 * @return Name of this device.
 */
QString Device::name() const
{
   EDEBUG_FUNC(this);
   return _name;
}






/*!
 * Returns the OpenCL device type of this device.
 *
 * @return OpenCL device type of this device.
 */
OpenCL::Device::Type Device::type() const
{
   EDEBUG_FUNC(this);
   return _type;
}






/*!
 * Returns the OpenCL platform ID that this device belongs to.
 *
 * @return OpenCL platform ID of this device.
 */
cl_platform_id Device::platform() const
{
   EDEBUG_FUNC(this);
   return _platform;
}






/*!
 * Gets string based information about this OpenCL device with the given info
 * type.
 *
 * @param type The information type that is returned.
 *
 * @return Information of this device with the given info type.
 */
QString Device::getStringInfo(cl_device_info type) const
{
   // Add the debug header.
   EDEBUG_FUNC(this,type);

   // Query the size of the information requested, then get the information as a C
   // style string, and then return it as a qt string. If any OpenCL error occurs
   // then throw an exception.
   size_t size;
   cl_int code {clGetDeviceInfo(_id,type,0,nullptr,&size)};
   if ( code != CL_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      fillException(&e,code);
      throw e;
   }
   Q_ASSERT(size < std::numeric_limits<int>::max());
   QByteArray data(static_cast<int>(size),'\0');
   code = clGetDeviceInfo(_id,type,size,data.data(),nullptr);
   if ( code != CL_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      fillException(&e,code);
      throw e;
   }
   return QString(data);
}

}
