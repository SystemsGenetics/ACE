#include "opencl_platform.h"
#include "opencl_device.h"
#include "opencl_common.h"
#include "eexception.h"
#include "edebug.h"



using namespace OpenCL;
//



/*!
 * Global pointer list of OpenCL platform objects that is available on this system. 
 */
QList<OpenCL::Platform*>* Platform::_platforms {nullptr};






/*!
 * Returns the number of OpenCL platforms that exist. 
 *
 * @return Number of OpenCL platforms. 
 */
int Platform::size()
{
   EDEBUG_FUNC()

   // Populate platform list and return its size. 
   populate();
   return _platforms->size();
}






/*!
 * Returns a pointer to the OpenCL platform with the given index. If the index is 
 * out of range an exception is thrown. 
 *
 * @param index Index of OpenCL platform whose reference is returned. 
 *
 * @return Pointer to OpenCL platform with given index. 
 */
OpenCL::Platform* Platform::get(int index)
{
   EDEBUG_FUNC(index)

   // Populate platform list and make sure given index is within range. If the given 
   // index is out of range then throw an exception, else go to the next step. 
   populate();
   if ( index < 0 || index >= _platforms->size() )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Out Of Range"));
      e.setDetails(tr("OpenCL platform index %1 is out of range (%2 platforms exist).")
                   .arg(index)
                   .arg(_platforms->size()));
      throw e;
   }

   // Return pointer to platform with the given index. 
   return _platforms->at(index);
}






/*!
 * Returns the OpenCL platform ID for this platform object. 
 *
 * @return OpenCL platform ID for this object. 
 */
cl_platform_id Platform::id() const
{
   EDEBUG_FUNC(this)

   return _id;
}






/*!
 * Returns the profile of this platform. 
 *
 * @return Profile of this platform. 
 */
QString Platform::profile() const
{
   EDEBUG_FUNC(this)

   return _profile;
}






/*!
 * Returns the OpenCL version used by this platform. 
 *
 * @return OpenCL version used by this platform. 
 */
QString Platform::version() const
{
   EDEBUG_FUNC(this)

   return _version;
}






/*!
 * Returns the name of this platform. 
 *
 * @return Name of this platform. 
 */
QString Platform::name() const
{
   EDEBUG_FUNC(this)

   return _name;
}






/*!
 * Returns the vendor of this platform. 
 *
 * @return Vendor of this platform. 
 */
QString Platform::vendor() const
{
   EDEBUG_FUNC(this)

   return _vendor;
}






/*!
 * Returns the OpenCL extensions this platform supports. 
 *
 * @return OpenCL extensions this platform supports. 
 */
const QStringList& Platform::extensions() const
{
   EDEBUG_FUNC(this)

   return _extensions;
}






/*!
 * Returns the number of OpenCL devices this platform contains. 
 *
 * @return Number of OpenCL devices this platform contains. 
 */
int Platform::deviceSize() const
{
   EDEBUG_FUNC(this)

   return _devices.size();
}






/*!
 * Returns a pointer to the OpenCL device contained in this platform with the given 
 * index. If the index is out of range then an exception is thrown. 
 *
 * @param index Index of OpenCL device of this platform whose pointer is returned. 
 *
 * @return Pointer to OpenCL device of this platform with the given index. 
 */
OpenCL::Device* Platform::device(int index) const
{
   EDEBUG_FUNC(this,index)

   // If the given index is out of range then throw an exception, else go to the next 
   // step. 
   if ( index < 0 || index >= _devices.size() )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Out Of Range"));
      e.setDetails(tr("OpenCL device index %1 is out of range (%2 devices exist).")
                   .arg(index)
                   .arg(_devices.size()));
      throw e;
   }

   // Return pointer to the OpenCL device of this platform with the given index. 
   return _devices.at(index);
}






/*!
 * Populates the global list of OpenCL platforms if it has not already been 
 * populated. 
 */
void Platform::populate()
{
   EDEBUG_FUNC()

   // If the global pointer to the platforms list is null then go to the next step, 
   // else do nothing and exit. 
   if ( !_platforms )
   {
      // Query the list of OpenCL platform IDs and create a new OpenCL platform object 
      // for each ID, adding its pointer to the global list of platforms. If any OpenCL 
      // error occurs then set the list of platforms to empty and return. 
      _platforms = new QList<Platform*>();
      cl_uint size;
      cl_int code {clGetPlatformIDs(0,nullptr,&size)};
      if ( code != CL_SUCCESS )
      {
         return;
      }
      cl_platform_id platforms[size];
      code = clGetPlatformIDs(size,platforms,nullptr);
      if ( code != CL_SUCCESS )
      {
         return;
      }
      for (cl_uint i = 0; i < size ;++i)
      {
         *_platforms << new Platform(platforms[i]);
      }
   }
}






/*!
 * Constructs a new OpenCL platform using the given OpenCL platform ID. This is 
 * only used internally by the static methods which initially populate the list of 
 * platforms. 
 *
 * @param id OpenCL platform ID used to construct this new platform object. 
 *
 * @param parent Optional parent for this object. 
 */
Platform::Platform(cl_platform_id id, QObject* parent):
   QObject(parent),
   _id(id),
   _profile(getInfo(CL_PLATFORM_PROFILE)),
   _version(getInfo(CL_PLATFORM_VERSION)),
   _name(getInfo(CL_PLATFORM_NAME)),
   _vendor(getInfo(CL_PLATFORM_VENDOR)),
   _extensions(getInfo(CL_PLATFORM_EXTENSIONS).split(' '))
{
   EDEBUG_FUNC(this,(void*)id,parent)

   populateDevices();
}






/*!
 * Populates this platform's list of OpenCL devices. 
 */
void Platform::populateDevices()
{
   EDEBUG_FUNC(this)

   // Query the list of OpenCL device IDs for this OpenCL platform, creating a new 
   // OpenCL device with each ID and appending their pointer to this platform's list 
   // of devices. If any OpenCL error occurs then throw an exception. 
   cl_uint size;
   cl_int code {clGetDeviceIDs(_id,CL_DEVICE_TYPE_ALL,0,nullptr,&size)};
   if ( code != CL_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      fillException(&e,code);
      throw e;
   }
   cl_device_id devices[size];
   code = clGetDeviceIDs(_id,CL_DEVICE_TYPE_ALL,size,devices,nullptr);
   if ( code != CL_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      fillException(&e,code);
      throw e;
   }
   for (cl_uint i = 0; i < size ;++i)
   {
      _devices << new Device(devices[i],this);
   }
}






/*!
 * Gets string based information about this OpenCL platform with the given info 
 * type. 
 *
 * @param type The information type that is returned. 
 *
 * @return Information of this platform with the given info type. 
 */
QString Platform::getInfo(cl_platform_info type) const
{
   EDEBUG_FUNC(this,type)

   // Query the size of the information requested, then get the information as a C 
   // style string, and then return it as a qt string. If any OpenCL error occurs 
   // then throw and exception. 
   size_t size;
   cl_int code {clGetPlatformInfo(_id,type,0,nullptr,&size)};
   if ( code != CL_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      fillException(&e,code);
      throw e;
   }
   char info[size];
   code = clGetPlatformInfo(_id,type,size,info,nullptr);
   if ( code != CL_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      fillException(&e,code);
      throw e;
   }
   return QString(info);
}
