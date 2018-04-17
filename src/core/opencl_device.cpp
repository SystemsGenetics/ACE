#include "opencl_device.h"



using namespace OpenCL;
//






/*!
 *
 * @param id  
 *
 * @param parent  
 */
Device::Device(cl_device_id id, QObject* parent):
   QObject(parent),
   _id(id),
   _available(getInfo<bool,cl_bool>(CL_DEVICE_AVAILABLE)),
   _compiler(getInfo<bool,cl_bool>(CL_DEVICE_COMPILER_AVAILABLE)),
   _extensions(getStringInfo(CL_DEVICE_EXTENSIONS).split(' ')),
   _globalMemorySize(getInfo<qint64,cl_ulong>(CL_DEVICE_GLOBAL_MEM_SIZE)),
   _localMemorySize(getInfo<qint64,cl_ulong>(CL_DEVICE_LOCAL_MEM_SIZE)),
   _computeUnitSize(getInfo<int,cl_uint>(CL_DEVICE_MAX_COMPUTE_UNITS)),
   _clockFrequency(getInfo<int,cl_uint>(CL_DEVICE_MAX_CLOCK_FREQUENCY)),
   _name(getStringInfo(CL_DEVICE_NAME))
{
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
 */
cl_device_id Device::id() const
{
   return _id;
}






/*!
 */
bool Device::isAvailable() const
{
   return _available;
}






/*!
 */
bool Device::hasCompiler() const
{
   return _compiler;
}






/*!
 */
const QStringList& Device::extensions() const
{
   return _extensions;
}






/*!
 */
qint64 Device::globalMemorySize() const
{
   return _globalMemorySize;
}






/*!
 */
qint64 Device::localMemorySize() const
{
   return _localMemorySize;
}






/*!
 */
int Device::computeUnitSize() const
{
   return _computeUnitSize;
}






/*!
 */
int Device::clockFrequency() const
{
   return _clockFrequency;
}






/*!
 */
QString Device::name() const
{
   return _name;
}






/*!
 */
OpenCL::Device::Type Device::type() const
{
   return _type;
}






/*!
 *
 * @tparam R  
 *
 * @tparam T  
 *
 * @param type  
 */
template<class R,class T> R Device::getInfo(cl_device_info type) const
{
   size_t size {1};
   T data;
   cl_int code {clGetDeviceInfo(_id,type,size,&data,NULL)};
   if ( code != CL_SUCCESS )
   {
      ;
   }
   return static_cast<R>(data);
}






/*!
 *
 * @param type  
 */
QString Device::getStringInfo(cl_device_info type) const
{
   size_t size;
   cl_int code {clGetDeviceInfo(_id,type,0,NULL,&size)};
   if ( code != CL_SUCCESS )
   {
      ;
   }
   char data[size];
   code = clGetDeviceInfo(_id,type,size,data,NULL);
   if ( code != CL_SUCCESS )
   {
      ;
   }
   return QString(data);
}
