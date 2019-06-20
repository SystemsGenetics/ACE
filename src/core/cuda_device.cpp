#include "cuda_device.h"



using namespace CUDA;
//






/*!
 * Global pointer list of CUDA devices that are available on this system.
 */
QList<CUDA::Device*>* Device::_devices {nullptr};






/*!
 * Returns the number of available CUDA devices.
 *
 * @return Number of available CUDA devices. 
 */
int Device::size()
{
   // Populate device list and return its size. 
   populate();
   return _devices->size();
}






/*!
 * Returns a pointer to the CUDA device with the given index. If the index is
 * out of range an exception is thrown.
 *
 * @param index Index of CUDA device whose pointer is returned.
 *
 * @return Pointer to CUDA device with given index.
 */
CUDA::Device* Device::get(int index)
{
   // Populate device list and make sure given index is within range. If the given 
   // index is out of range then throw an exception, else go to the next step. 
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

   // Return pointer to device with the given index. 
   return _devices->at(index);
}






/*!
 * Constructs a new CUDA device object with the given device index.
 *
 * @param ordinal Device index used to construct this new device object. 
 */
Device::Device(int ordinal)
{
   // Get a device pointer for the given device index. If this command fails
   // then throw an exception.
   CUresult result = cuDeviceGet(&_id, ordinal);
   if ( result != CUDA_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      throwError(&e,result);
   }
}






/*!
 * Returns the name of this device. 
 *
 * @return Name of this device. 
 */
QString Device::name() const
{
   // Read the name of this device into a character buffer and return it as a
   // string. If this command fails then throw an exception.
   char name[256];
   CUresult result = cuDeviceGetName(name, sizeof(name), _id);
   if ( result != CUDA_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      throwError(&e,result);
   }

   return QString(name);
}






/*!
 * Returns an attribute of this device. Refer to the CUDA Toolkit Documentation
 * for more information on the available kernel attributes.
 *
 * @param attribute Enumerated value for a CUDA device attribute.
 *
 * @return Integer value of the given attribute.
 */
int Device::getAttribute(CUdevice_attribute attribute) const
{
   // Get the value of the given device attribute. If this command fails then
   // throw an exception.
   int value;
   CUresult result = cuDeviceGetAttribute(&value, attribute, _id);
   if ( result != CUDA_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      throwError(&e,result);
   }

   return value;
}






/*!
 * Returns the global memory size of this device in bytes.
 *
 * @return Global memory size of this device in bytes.
 */
size_t Device::getGlobalMemorySize() const
{
   // Get the global memory size of this device. If this command fails then
   // throw an exception.
   size_t bytes;
   CUresult result = cuDeviceTotalMem(&bytes, _id);
   if ( result != CUDA_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      throwError(&e,result);
   }

   return bytes;
}






/*!
 * Populates the global list of CUDA devices if it has not already been
 * populated. This function should be called before any other function that
 * uses the CUDA Driver API, since this function initializes the CUDA Driver API.
 */
void Device::populate()
{
   // If the global pointer to the devices list is null then go to the next step, 
   // else do nothing and exit. 
   if ( !_devices )
   {
      // Initialize the CUDA Driver API. This function must be called before any
      // other function in the CUDA Driver API. If initialization fails then
      // throw an exception.
      CUresult result = cuInit(0);
      if ( result != CUDA_SUCCESS )
      {
         E_MAKE_EXCEPTION(e);
         throwError(&e,result);
      }

      // Initialize the global device list.
      _devices = new QList<Device*>();

      // Query the number of CUDA devices. If the query fails then throw an
      // exception.
      int count;
      result = cuDeviceGetCount(&count);
      if ( result != CUDA_SUCCESS )
      {
         E_MAKE_EXCEPTION(e);
         throwError(&e,result);
      }

      // For each device index, create a new device object and append it to the
      // global device list.
      for ( int i = 0; i < count; ++i )
      {
         *_devices << new Device(i);
      }
   }
}
