#include "opencl_platform.h"
#include "opencl_device.h"



using namespace OpenCL;
//



/*!
 */
QList<OpenCL::Platform*>* Platform::_platforms {nullptr};






/*!
 */
int Platform::size()
{
   populate();
   return _platforms->size();
}






/*!
 *
 * @param index  
 */
OpenCL::Platform* Platform::get(int index)
{
   populate();
   if ( index < 0 || index >= _platforms->size() )
   {
      ;
   }
   return _platforms->at(index);
}






/*!
 *
 * @param id  
 *
 * @param parent  
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
   populateDevices();
}






/*!
 */
QString Platform::profile() const
{
   return _profile;
}






/*!
 */
QString Platform::version() const
{
   return _version;
}






/*!
 */
QString Platform::name() const
{
   return _name;
}






/*!
 */
QString Platform::vendor() const
{
   return _vendor;
}






/*!
 */
const QStringList& Platform::extensions() const
{
   return _extensions;
}






/*!
 */
int Platform::deviceSize() const
{
   return _devices.size();
}






/*!
 *
 * @param index  
 */
OpenCL::Device* Platform::device(int index) const
{
   if ( index < 0 || index >= _devices.size() )
   {
      ;
   }
   return _devices.at(index);
}






/*!
 */
void Platform::populate()
{
   if ( !_platforms )
   {
      cl_uint size;
      cl_int code {clGetPlatformIDs(0,NULL,&size)};
      if ( code != CL_SUCCESS )
      {
         ;
      }
      cl_platform_id platforms[size];
      code = clGetPlatformIDs(size,platforms,NULL);
      if ( code != CL_SUCCESS )
      {
         ;
      }
      _platforms = new QList<Platform*>();
      for (cl_uint i = 0; i < size ;++i)
      {
         *_platforms << new Platform(platforms[i]);
      }
   }
}






/*!
 */
void Platform::populateDevices()
{
   cl_uint size;
   cl_int code {clGetDeviceIDs(_id,CL_DEVICE_TYPE_ALL,0,NULL,&size)};
   if ( code != CL_SUCCESS )
   {
      ;
   }
   cl_device_id devices[size];
   code = clGetDeviceIDs(_id,CL_DEVICE_TYPE_ALL,size,devices,NULL);
   if ( code != CL_SUCCESS )
   {
      ;
   }
   for (cl_uint i = 0; i < size ;++i)
   {
      _devices << new Device(devices[i],this);
   }
}






/*!
 *
 * @param type  
 */
QString Platform::getInfo(cl_platform_info type) const
{
   size_t size;
   cl_int code {clGetPlatformInfo(_id,type,0,NULL,&size)};
   if ( code != CL_SUCCESS )
   {
      ;
   }
   char info[size];
   code = clGetPlatformInfo(_id,type,size,info,NULL);
   if ( code != CL_SUCCESS )
   {
      ;
   }
   return QString(info);
}
