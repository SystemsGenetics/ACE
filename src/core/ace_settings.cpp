#include "ace_settings.h"
#include <QSettings>
#include "opencl_platform.h"
#include "eexception.h"



using namespace Ace;
//



/*!
 * Major version of the application using ACE. 
 */
int Settings::_appMajorVersion {0};
/*!
 * Minor version of the application using ACE. 
 */
int Settings::_appMinorVersion {0};
/*!
 * Revision of the application using ACE. 
 */
int Settings::_appRevision {0};
/*!
 * Organization name of the application using ACE. 
 */
QString Settings::_organization;
/*!
 * Name of the application using ACE. 
 */
QString Settings::_application;
/*!
 * The default chunk working directory value. 
 */
const char* Settings::_chunkDirDefault {"."};
/*!
 * The default chunk file prefix value. 
 */
const char* Settings::_chunkPrefixDefault {"chunk"};
/*!
 * The default chunk file extension value. 
 */
const char* Settings::_chunkExtensionDefault {"abd"};
/*!
 * The qt settings key used to persistently store the platform index value. 
 */
const char* Settings::_platformKey {"opencl.platform"};
/*!
 * The qt settings key used to persistently store the device index value. 
 */
const char* Settings::_deviceKey {"opencl.device"};
/*!
 * The qt settings key used to persistently store the thread size value. 
 */
const char* Settings::_threadSizeKey {"opencl.thread.size"};
/*!
 * The qt settings key used to persistently store the buffer size value. 
 */
const char* Settings::_bufferSizeKey {"mpi.buffersize"};
/*!
 * The qt settings key used to persistently store the chunk working directory 
 * value. 
 */
const char* Settings::_chunkDirKey {"chunk.directory"};
/*!
 * The qt settings key used to persistently store the chunk file prefix value. 
 */
const char* Settings::_chunkPrefixKey {"chunk.prefix"};
/*!
 * The qt settings key used to persistently store the chunk file extension value. 
 */
const char* Settings::_chunkExtensionKey {"chunk.extension"};
/*!
 * Points to the global singleton instance of this class. 
 */
Settings* Settings::_instance {nullptr};






/*!
 * Returns the version of the ACE library as a string. 
 *
 * @return Version of ACE library as a string. 
 */
QString Settings::versionString()
{
   return QString::number(_majorVersion).append(".")
                                        .append(QString::number(_minorVersion))
                                        .append(".")
                                        .append(QString::number(_revision));
}






/*!
 * Returns the organization of the application using ACE. 
 *
 * @return Organization of the application using ACE. 
 */
QString Settings::organization()
{
   return _organization;
}






/*!
 * Returns the name of the application using ACE. 
 *
 * @return Name of the application using ACE. 
 */
QString Settings::application()
{
   return _application;
}






/*!
 * Returns the version of the application using ACE as a string. 
 *
 * @return Version of the application using ACE as a string. 
 */
QString Settings::appVersionString()
{
   return QString::number(_appMajorVersion).append(".")
                                           .append(QString::number(_appMinorVersion))
                                           .append(".")
                                           .append(QString::number(_appRevision));
}






/*!
 * Returns the major version of the application using ACE. 
 *
 * @return Major version of the application using ACE. 
 */
int Settings::appMajorVersion()
{
   return _appMajorVersion;
}






/*!
 * Returns the minor version of the application using ACE. 
 *
 * @return Minor version of the application using ACE. 
 */
int Settings::appMinorVersion()
{
   return _appMinorVersion;
}






/*!
 * The revision of the application using ACE. 
 *
 * @return Revision of the application using ACE. 
 */
int Settings::appRevision()
{
   return _appRevision;
}






/*!
 * Initializes and sets the organization name, name, and version numbers of the 
 * application using ACE. This must be called before the settings singleton 
 * instance is created. The application name cannot be "ace" or an exception is 
 * thrown. 
 *
 * @param organization Organization name of the application using ACE. 
 *
 * @param application Name of the application using ACE. 
 *
 * @param majorVersion Major version of the application using ACE. 
 *
 * @param minorVersion Minor version of the application using ACE. 
 *
 * @param revision Revision of the application using ACE. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given application name is equal to ace then throw an exception, else 
 *    go to the next step. 
 *
 * 2. Set the organization name, name, and version numbers from the given 
 *    application info. 
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
 * Returns a reference to the singleton instance of this class. If the singleton 
 * instance has not yet been created then it is before returning. 
 *
 * @return Reference to the singleton instance of this class. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the global pointer to the singleton instance of this class is null then 
 *    create a new instance and set it to the global pointer. 
 *
 * 2. Return a reference to the singleton instance at the global pointer. 
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
 * Returns the platform index of the preferred OpenCL device. 
 *
 * @return Platform index of the preferred OpenCL device. 
 */
int Settings::platform() const
{
   return _platform;
}






/*!
 * Returns the device index of the preferred OpenCL device. 
 *
 * @return Device index of the preferred OpenCL device. 
 */
int Settings::device() const
{
   return _device;
}






/*!
 * Returns a pointer to the preferred OpenCL device or null if the preference is 
 * none. 
 *
 * @return Pointer to preferred OpenCL device or null if none is preferred. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the platform index is out of range or the device index is out of range 
 *    then return a null pointer, else return a pointer to the device with the 
 *    platform and device indexes. 
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
 * Returns the thread size for accelerated runs. 
 *
 * @return Thread size for accelerated runs. 
 */
int Settings::threadSize() const
{
   return _threadSize;
}






/*!
 * Returns the MPI buffer size used to determine the number of blocks are buffered 
 * for input to each slave node. 
 *
 * @return MPI buffer size. 
 */
int Settings::bufferSize() const
{
   return _bufferSize;
}






/*!
 * Returns the working directory path for chunk runs. 
 *
 * @return Working directory path for chunk runs. 
 */
QString Settings::chunkDir() const
{
   return _chunkDir;
}






/*!
 * Returns the chunk file name prefix. 
 *
 * @return Chunk file name prefix. 
 */
QString Settings::chunkPrefix() const
{
   return _chunkPrefix;
}






/*!
 * Returns the chunk file extension. 
 *
 * @return Chunk file extension. 
 */
QString Settings::chunkExtension() const
{
   return _chunkExtension;
}






/*!
 * Sets the platform index for the preferred OpenCL device. 
 *
 * @param index Index of the platform of the preferred OpenCL device. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the new given platform index is different from the current platform index 
 *    then set it to the new one and set the value in persistent storage. 
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
 * Sets the index of the preferred OpenCL device. 
 *
 * @param index Index of the preferred OpenCL device. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the new given device index is different from the current device index then 
 *    set it to the new one and set the value in persistent storage. 
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
 * Sets the thread size for accelerated runs. The size must be one or greater or an 
 * exception is thrown. 
 *
 * @param size Thread size for accelerated runs. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the new given thread size is less than one then throw an exception, else 
 *    go to the next step. 
 *
 * 2. If the new given thread size is different from the current thread size then 
 *    set it to the new one and set the value in persistent storage. 
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
 * Sets the MPI buffer size used to determine the number of blocks are buffered for 
 * input to each slave node. 
 *
 * @param size new MPI buffer size. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the new given buffer size is less than one then throw an exception, else 
 *    go to the next step. 
 *
 * 2. If the new given buffer size is different from the current buffer size then 
 *    set it to the new one and set the value in persistent storage. 
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
 * Sets the working directory for chunk runs. 
 *
 * @param path Working directory path for chunk runs. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the new given working directory path is different from the current working 
 *    directory path then set it to the new one and set the value in persistent 
 *    storage. 
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
 * Sets the chunk file name prefix. 
 *
 * @param prefix New chunk file name prefix. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the new given chunk file prefix is different from the current chunk file 
 *    prefix then set it to the new one and set the value in persistent storage. 
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
 * Sets the chunk file extension. 
 *
 * @param extension New chunk file extension. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the new given chunk file extension is different from the current chunk 
 *    file extension then set it to the new one and set the value in persistent 
 *    storage. 
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
 * Constructs a new settings object. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Initialize all settings this class stores, setting them to their default 
 *    values if their values do not exist in qt settings with their given key. 
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
 * Save the given value with the given key to qt settings. 
 *
 * @param key The key used to save the given value. 
 *
 * @param value The given value that is saved. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Initialize qt settings and save the given value to the given key. 
 */
void Settings::setValue(const QString& key, const QVariant& value)
{
   QSettings settings(_organization,_application);
   settings.setValue(key,value);
}
