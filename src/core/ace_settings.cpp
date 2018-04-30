#include "ace_settings.h"
#include <QSettings>
#include "opencl_platform.h"
#include "eexception.h"



using namespace Ace;
//



/*!
 */
int Settings::_appMajorVersion {0};
/*!
 */
int Settings::_appMinorVersion {0};
/*!
 */
int Settings::_appRevision {0};
/*!
 */
const char* Settings::_chunkDirDefault {"."};
/*!
 */
const char* Settings::_chunkPrefixDefault {"chunk"};
/*!
 */
const char* Settings::_chunkExtensionDefault {"abd"};
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
const char* Settings::_bufferSizeKey {"mpi.buffersize"};
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
QString Settings::versionString()
{
   return QString::number(_majorVersion).append(".")
                                        .append(QString::number(_minorVersion))
                                        .append(".")
                                        .append(QString::number(_revision));
}






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
 */
QString Settings::appVersionString()
{
   return QString::number(_appMajorVersion).append(".")
                                           .append(QString::number(_appMinorVersion))
                                           .append(".")
                                           .append(QString::number(_appRevision));
}






/*!
 */
int Settings::appMajorVersion()
{
   return _appMajorVersion;
}






/*!
 */
int Settings::appMinorVersion()
{
   return _appMinorVersion;
}






/*!
 */
int Settings::appRevision()
{
   return _appRevision;
}






/*!
 *
 * @param organization  
 *
 * @param application  
 *
 * @param majorVersion  
 *
 * @param minorVersion  
 *
 * @param revision  
 */
void Settings::initialize(QString organization, QString application, int majorVersion, int minorVersion, int revision)
{
   if ( application == QString("ace") )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid Argument"));
      e.setDetails(QObject::tr("Cannot set application name to 'ace'."));
      throw e;
   }
   _organization = organization;
   _application = application;
   _appMajorVersion = majorVersion;
   _appMinorVersion = minorVersion;
   _appRevision = revision;
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
int Settings::bufferSize() const
{
   return _bufferSize;
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
   if ( size < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid Argument"));
      e.setDetails(QObject::tr("Cannot set thread size to %1 (1 is smallest allowed).").arg(size));
      throw e;
   }
   if ( size != _threadSize )
   {
      _threadSize = size;
      setValue(_threadSizeKey,_threadSize);
   }
}






/*!
 *
 * @param size  
 */
void Settings::setBufferSize(int size)
{
   if ( size < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid Argument"));
      e.setDetails(QObject::tr("Cannot set buffer size to %1 (1 is smallest allowed).").arg(size));
      throw e;
   }
   if ( size != _bufferSize )
   {
      _bufferSize = size;
      setValue(_bufferSizeKey,_bufferSize);
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
   _platform = settings.value(_platformKey,_platformDefault).toInt();
   _device = settings.value(_deviceKey,_deviceDefault).toInt();
   _threadSize = settings.value(_threadSizeKey,_threadSizeDefault).toInt();
   _bufferSize = settings.value(_bufferSizeKey,_bufferSizeDefault).toInt();
   _chunkDir = settings.value(_chunkDirKey,_chunkDirDefault).toString();
   _chunkPrefix = settings.value(_chunkPrefixKey,_chunkPrefixDefault).toString();
   _chunkExtension = settings.value(_chunkExtensionKey,_chunkExtensionDefault).toString();
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
