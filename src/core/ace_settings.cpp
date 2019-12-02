#include "ace_settings.h"
#include <QSettings>
#include "cuda_device.h"
#include "opencl_platform.h"
#include "eexception.h"



namespace Ace
{



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
 */
const bool Settings::_loggingEnabledDefault {false};
/*!
 */
const int Settings::_loggingPortDefault {40000};
/*!
 * The qt settings key used to persistently store the CUDA device index value.
 */
const char* Settings::_cudaDeviceKey {"cuda.device"};
/*!
 * The qt settings key used to persistently store the platform index value.
 */
const char* Settings::_openCLPlatformKey {"opencl.platform"};
/*!
 * The qt settings key used to persistently store the device index value.
 */
const char* Settings::_openCLDeviceKey {"opencl.device"};
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
 * The qt settings key used to persistently store the chunk file extension
 * value.
 */
const char* Settings::_chunkExtensionKey {"chunk.extension"};
/*!
 */
const char* Settings::_loggingEnabledKey {"logging.enabled"};
/*!
 */
const char* Settings::_loggingPortKey {"logging.port"};
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
 */
void Settings::initialize(QString organization, QString application, int majorVersion, int minorVersion, int revision)
{
   // If the given application name is equal to ace then throw an exception, else go
   // to the next step.
   if ( application == QString("ace") )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid Argument"));
      e.setDetails(QObject::tr("Cannot set application name to 'ace'."));
      throw e;
   }

   // Set the organization name, name, and version numbers from the given application
   // info.
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
 */
Settings& Settings::instance()
{
   // If the global pointer to the singleton instance of this class is null then
   // create a new instance and set it to the global pointer.
   if ( !_instance )
   {
      _instance = new Settings;
   }

   // Return a reference to the singleton instance at the global pointer.
   return *_instance;
}






/*!
 * Returns the index of the preferred CUDA device.
 *
 * @return Index of the preferred CUDA device.
 */
int Settings::cudaDevice() const
{
   return _cudaDevice;
}






/*!
 * Returns a pointer to the preferred CUDA device or null if the preference is
 * none.
 *
 * @return Pointer to preferred CUDA device or null if none is preferred.
 */
CUDA::Device* Settings::cudaDevicePointer() const
{
   // If the device index is out of range then return a null pointer, else return a
   // pointer to the device with the device index.
   if ( _cudaDevice < 0 || _cudaDevice >= CUDA::Device::get(_cudaDevice)->size() )
   {
      return nullptr;
   }
   return CUDA::Device::get(_cudaDevice);
}






/*!
 * Returns the platform index of the preferred OpenCL device.
 *
 * @return Platform index of the preferred OpenCL device.
 */
int Settings::openCLPlatform() const
{
   return _openCLPlatform;
}






/*!
 * Returns the device index of the preferred OpenCL device.
 *
 * @return Device index of the preferred OpenCL device.
 */
int Settings::openCLDevice() const
{
   return _openCLDevice;
}






/*!
 * Returns a pointer to the preferred OpenCL device or null if the preference is
 * none.
 *
 * @return Pointer to preferred OpenCL device or null if none is preferred.
 */
OpenCL::Device* Settings::openCLDevicePointer() const
{
   // If the platform index is out of range or the device index is out of range then
   // return a null pointer, else return a pointer to the device with the platform
   // and device indexes.
   if ( _openCLPlatform < 0
        || _openCLDevice < 0
        || _openCLPlatform >= OpenCL::Platform::size()
        || _openCLDevice >= OpenCL::Platform::get(_openCLPlatform)->deviceSize() )
   {
      return nullptr;
   }
   return OpenCL::Platform::get(_openCLPlatform)->device(_openCLDevice);
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
 * Returns the MPI buffer size used to determine the number of blocks are
 * buffered for input to each slave node.
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
 * Returns the logging enabled state.
 *
 * @return Logging enabled state.
 */
bool Settings::loggingEnabled() const
{
   return _loggingEnabled;
}






/*!
 * Returns the logging port number.
 *
 * @return Logging port number.
 */
int Settings::loggingPort() const
{
   return _loggingPort;
}






/*!
 * Sets the index for the preferred CUDA device.
 *
 * @param index Index of the preferred CUDA device.
 */
void Settings::setCUDADevice(int index)
{
   // If the new given device index is different from the current device index then
   // set it to the new one and set the value in persistent storage.
   if ( index != _cudaDevice )
   {
      _cudaDevice = index;
      setValue(_cudaDeviceKey,_cudaDevice);
   }
}






/*!
 * Sets the platform index for the preferred OpenCL device.
 *
 * @param index Index of the platform of the preferred OpenCL device.
 */
void Settings::setOpenCLPlatform(int index)
{
   // If the new given platform index is different from the current platform index
   // then set it to the new one and set the value in persistent storage.
   if ( index != _openCLPlatform )
   {
      _openCLPlatform = index;
      setValue(_openCLPlatformKey,_openCLPlatform);
   }
}






/*!
 * Sets the index of the preferred OpenCL device.
 *
 * @param index Index of the preferred OpenCL device.
 */
void Settings::setOpenCLDevice(int index)
{
   // If the new given device index is different from the current device index then
   // set it to the new one and set the value in persistent storage.
   if ( index != _openCLDevice )
   {
      _openCLDevice = index;
      setValue(_openCLDeviceKey,_openCLDevice);
   }
}






/*!
 * Sets the thread size for accelerated runs. The size must be one or greater or
 * an exception is thrown.
 *
 * @param size Thread size for accelerated runs.
 */
void Settings::setThreadSize(int size)
{
   // If the new given thread size is less than one then throw an exception, else go
   // to the next step.
   if ( size < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid Argument"));
      e.setDetails(QObject::tr("Cannot set thread size to %1 (1 is smallest allowed).").arg(size));
      throw e;
   }

   // If the new given thread size is different from the current thread size then set
   // it to the new one and set the value in persistent storage.
   if ( size != _threadSize )
   {
      _threadSize = size;
      setValue(_threadSizeKey,_threadSize);
   }
}






/*!
 * Sets the MPI buffer size used to determine the number of blocks are buffered
 * for input to each slave node.
 *
 * @param size new MPI buffer size.
 */
void Settings::setBufferSize(int size)
{
   // If the new given buffer size is less than one then throw an exception, else go
   // to the next step.
   if ( size < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Invalid Argument"));
      e.setDetails(QObject::tr("Cannot set buffer size to %1 (1 is smallest allowed).").arg(size));
      throw e;
   }

   // If the new given buffer size is different from the current buffer size then set
   // it to the new one and set the value in persistent storage.
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
 */
void Settings::setChunkDir(const QString& path)
{
   // If the new given working directory path is different from the current working
   // directory path then set it to the new one and set the value in persistent
   // storage.
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
 */
void Settings::setChunkPrefix(const QString& prefix)
{
   // If the new given chunk file prefix is different from the current chunk file
   // prefix then set it to the new one and set the value in persistent storage.
   if ( prefix != _chunkPrefix )
   {
      _chunkPrefix = prefix;
      setValue(_chunkPrefixKey,_chunkPrefix);
   }
}






/*!
 * Sets the chunk file extension.
 *
 * @param extension New chunk file extension.
 */
void Settings::setChunkExtension(const QString& extension)
{
   // If the new given chunk file extension is different from the current chunk file
   // extension then set it to the new one and set the value in persistent storage.
   if ( extension != _chunkExtension )
   {
      _chunkExtension = extension;
      setValue(_chunkExtensionKey,_chunkExtension);
   }
}






/*!
 * Set the logging enabled state.
 *
 * @param state New logging enabled state.
 */
void Settings::setLoggingEnabled(int state)
{
   // If the new value is different from the current value then set it to the new one
   // and set the value in persistent storage.
   if ( state != _loggingEnabled )
   {
      _loggingEnabled = state;
      setValue(_loggingEnabledKey,_loggingEnabled);
   }
}






/*!
 * Set the logging port.
 *
 * @param port New logging port number.
 */
void Settings::setLoggingPort(int port)
{
   // If the new value is different from the current value then set it to the new one
   // and set the value in persistent storage.
   if ( port != _loggingPort )
   {
      _loggingPort = port;
      setValue(_loggingPortKey,_loggingPort);
   }
}






/*!
 * Constructs a new settings object.
 */
Settings::Settings()
{
   // Initialize all settings this class stores, setting them to their default values
   // if their values do not exist in qt settings with their given key.
   QSettings settings(_organization,_application);
   _cudaDevice = settings.value(_cudaDeviceKey,_cudaDeviceDefault).toInt();
   _openCLPlatform = settings.value(_openCLPlatformKey,_openCLPlatformDefault).toInt();
   _openCLDevice = settings.value(_openCLDeviceKey,_openCLDeviceDefault).toInt();
   _threadSize = settings.value(_threadSizeKey,_threadSizeDefault).toInt();
   _bufferSize = settings.value(_bufferSizeKey,_bufferSizeDefault).toInt();
   _chunkDir = settings.value(_chunkDirKey,_chunkDirDefault).toString();
   _chunkPrefix = settings.value(_chunkPrefixKey,_chunkPrefixDefault).toString();
   _chunkExtension = settings.value(_chunkExtensionKey,_chunkExtensionDefault).toString();
   _loggingEnabled = settings.value(_loggingEnabledKey,_loggingEnabledDefault).toBool();
   _loggingPort = settings.value(_loggingPortKey,_loggingPortDefault).toInt();
}






/*!
 * Save the given value with the given key to qt settings.
 *
 * @param key The key used to save the given value.
 *
 * @param value The given value that is saved.
 */
void Settings::setValue(const QString& key, const QVariant& value)
{
   // Initialize qt settings and save the given value to the given key.
   QSettings settings(_organization,_application);
   settings.setValue(key,value);
}

}
