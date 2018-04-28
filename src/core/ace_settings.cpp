#include "ace_settings.h"
#include <QSettings>
#include "opencl_platform.h"



using namespace Ace;
//



/*!
 */
QString Settings::_organization;
/*!
 */
QString Settings::_application {"ace"};
/*!
 */
const char* Settings::_platformKey {"opencl.platform"};
/*!
 */
const char* Settings::_deviceKey {"opencl.device"};
/*!
 */
const char* Settings::_threadSizeKey {"opencl.thread.size"};
/*!
 */
const char* Settings::_chunkDirKey {"chunk.directory"};
/*!
 */
const char* Settings::_chunkPrefixKey {"chunk.prefix"};
/*!
 */
const char* Settings::_chunkExtensionKey {"chunk.extension"};
/*!
 */
Settings* Settings::_instance {nullptr};






/*!
 */
QString Settings::organization()
{
   return _organization;
}






/*!
 */
QString Settings::application()
{
   return _application;
}






/*!
 *
 * @param organization  
 *
 * @param application  
 */
void Settings::initialize(QString organization, QString application)
{
   _organization = organization;
   _application = application;
}






/*!
 */
Settings& Settings::instance()
{
   if ( !_instance )
   {
      _instance = new Settings;
   }
   return *_instance;
}






/*!
 */
int Settings::platform() const
{
   return _platform;
}






/*!
 */
int Settings::device() const
{
   return _device;
}






/*!
 */
OpenCL::Device* Settings::openCLDevicePointer() const
{
   if ( _platform < 0 || _device < 0 )
   {
      return nullptr;
   }
   if ( _platform >= OpenCL::Platform::size() )
   {
      return nullptr;
   }
   if ( _device >= OpenCL::Platform::get(_platform)->size() )
   {
      return nullptr;
   }
   return OpenCL::Platform::get(_platform)->device(_device);
}






/*!
 */
int Settings::threadSize() const
{
   return _threadSize;
}






/*!
 */
QString Settings::chunkDir() const
{
   return _chunkDir;
}






/*!
 */
QString Settings::chunkPrefix() const
{
   return _chunkPrefix;
}






/*!
 */
QString Settings::chunkExtension() const
{
   return _chunkExtension;
}






/*!
 *
 * @param index  
 */
void Settings::setPlatform(int index)
{
   if ( index != _platform )
   {
      _platform = index;
      setValue(_platformKey,_platform);
   }
}






/*!
 *
 * @param index  
 */
void Settings::setDevice(int index)
{
   if ( index != _device )
   {
      _device = index;
      setValue(_deviceKey,_device);
   }
}






/*!
 *
 * @param size  
 */
void Settings::setThreadSize(int size)
{
   if ( size != _threadSize )
   {
      _threadSize = size;
      setValue(_threadSizeKey,_threadSize);
   }
}






/*!
 *
 * @param path  
 */
void Settings::setChunkDir(const QString& path)
{
   if ( path != _chunkDir )
   {
      _chunkDir = path;
      setValue(_chunkDirKey,_chunkDir);
   }
}






/*!
 *
 * @param prefix  
 */
void Settings::setChunkPrefix(const QString& prefix)
{
   if ( prefix != _chunkPrefix )
   {
      _chunkDir = prefix;
      setValue(_chunkPrefixKey,_chunkPrefix);
   }
}






/*!
 *
 * @param extension  
 */
void Settings::setChunkExtension(const QString& extension)
{
   if ( extension != _chunkExtension )
   {
      _chunkExtension = extension;
      setValue(_chunkExtensionKey,_chunkExtension);
   }
}






/*!
 */
Settings::Settings()
{
   QSettings settings(_organization,_application);
   _platform = settings.value(_platformKey).toInt();
   _device = settings.value(_deviceKey).toInt();
   _threadSize = settings.value(_threadSizeKey,8).toInt();
   _chunkDir = settings.value(_chunkDirKey,".").toString();
   _chunkPrefix = settings.value(_chunkPrefixKey,"chunk").toString();
   _chunkExtension = settings.value(_chunkExtensionKey,"abd").toString();
}






/*!
 *
 * @param key  
 *
 * @param value  
 */
void Settings::setValue(const QString& key, const QVariant& value)
{
   QSettings settings(_organization,_application);
   settings.setValue(key,value);
}
