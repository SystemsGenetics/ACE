#include "cuda_device.h"



using namespace CUDA;






/*!
 * Global list of CUDA devices that are available on this system.
 */
QList<CUDA::Device*>* Device::_devices {nullptr};






/*!
 * Returns the number of available CUDA devices.
 */
int Device::size()
{
   populate();
   return _devices->size();
}






/*!
 * Returns a reference to the CUDA device with the given index. If the index is
 * out of range an exception is thrown.
 *
 * @param index
 */
CUDA::Device* Device::get(int index)
{
   populate();
   if ( index < 0 || index >= _devices->size() )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Out Of Range"));
      e.setDetails(QObject::tr("CUDA device index %1 is out of range (%2 devices exist).")
                   .arg(index)
                   .arg(_devices->size()));
      throw e;
   }
   return _devices->at(index);
}






Device::Device(int ordinal)
{
   CUDA_SAFE_CALL(cuDeviceGet(&_id, ordinal));
}






/*!
 * Get the name of a CUDA device.
 */
QString Device::name() const
{
   char name[256];
   CUDA_SAFE_CALL(cuDeviceGetName(name, sizeof(name), _id));

   return QString(name);
}






/*!
 * Get an attribute of a CUDA device.
 *
 * @param attribute
 */
int Device::getAttribute(CUdevice_attribute attribute) const
{
   int value;
   CUDA_SAFE_CALL(cuDeviceGetAttribute(&value, attribute, _id));

   return value;
}






/*!
 * Get the global memory size of a CUDA device in bytes.
 */
size_t Device::getGlobalMemorySize() const
{
   size_t bytes;
   CUDA_SAFE_CALL(cuDeviceTotalMem(&bytes, _id));

   return bytes;
}






/*!
 * Populates the global list of CUDA devices if it has not already been
 * populated.
 */
void Device::populate()
{
   if ( !_devices )
   {
      // initialize CUDA Driver API
      CUDA_SAFE_CALL(cuInit(0));

      // initialize device list
      _devices = new QList<Device*>();

      // get device count
      int count;
      CUDA_SAFE_CALL(cuDeviceGetCount(&count));

      // populate device list
      for ( int i = 0; i < count; ++i )
      {
         *_devices << new Device(i);
      }
   }
}
