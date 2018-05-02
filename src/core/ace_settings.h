#ifndef ACE_SETTINGS_H
#define ACE_SETTINGS_H
#include <QString>
#include "opencl.h"



class QSettings;
class QVariant;
//



namespace Ace
{
   /*!
    * This is a singleton class that holds all global settings for ACE and the program 
    * that uses it. Before calling on the instance of this class the static initialize 
    * method must be called first or this class will fail to getting the correct 
    * settings for the program using ACE. If any persistent setting is updated this 
    * class will save it using a qt settings object so it persists through to other 
    * instances of the program. 
    */
   class Settings
   {
   public:
      static QString versionString();
      static QString organization();
      static QString application();
      static QString appVersionString();
      static int appMajorVersion();
      static int appMinorVersion();
      static int appRevision();
      static void initialize(QString organization, QString application, int majorVersion, int minorVersion, int revision);
      static Settings& instance();
      /*!
       * The major version of the ACE library. 
       */
      constexpr static int _majorVersion {0};
      /*!
       * The minor version of the ACE library. 
       */
      constexpr static int _minorVersion {0};
      /*!
       * The revision of the ACE library. 
       */
      constexpr static int _revision {999};
      int platform() const;
      int device() const;
      OpenCL::Device* openCLDevicePointer() const;
      int threadSize() const;
      int bufferSize() const;
      QString chunkDir() const;
      QString chunkPrefix() const;
      QString chunkExtension() const;
      void setPlatform(int index);
      void setDevice(int index);
      void setThreadSize(int size);
      void setBufferSize(int size);
      void setChunkDir(const QString& path);
      void setChunkPrefix(const QString& prefix);
      void setChunkExtension(const QString& extension);
   private:
      /*!
       * Major version of the application using ACE. 
       */
      static int _appMajorVersion;
      /*!
       * Minor version of the application using ACE. 
       */
      static int _appMinorVersion;
      /*!
       * Revision of the application using ACE. 
       */
      static int _appRevision;
      /*!
       * Organization name of the application using ACE. 
       */
      static QString _organization;
      /*!
       * Name of the application using ACE. 
       */
      static QString _application;
      /*!
       * The default platform index value. 
       */
      constexpr static int _platformDefault {0};
      /*!
       * The default device index value. 
       */
      constexpr static int _deviceDefault {0};
      /*!
       * The default thread size value. 
       */
      constexpr static int _threadSizeDefault {4};
      /*!
       * The default buffer size value. 
       */
      constexpr static int _bufferSizeDefault {4};
      /*!
       * The default chunk working directory value. 
       */
      static const char* _chunkDirDefault;
      /*!
       * The default chunk file prefix value. 
       */
      static const char* _chunkPrefixDefault;
      /*!
       * The default chunk file extension value. 
       */
      static const char* _chunkExtensionDefault;
      /*!
       * The qt settings key used to persistently store the platform index value. 
       */
      static const char* _platformKey;
      /*!
       * The qt settings key used to persistently store the device index value. 
       */
      static const char* _deviceKey;
      /*!
       * The qt settings key used to persistently store the thread size value. 
       */
      static const char* _threadSizeKey;
      /*!
       * The qt settings key used to persistently store the buffer size value. 
       */
      static const char* _bufferSizeKey;
      /*!
       * The qt settings key used to persistently store the chunk working directory 
       * value. 
       */
      static const char* _chunkDirKey;
      /*!
       * The qt settings key used to persistently store the chunk file prefix value. 
       */
      static const char* _chunkPrefixKey;
      /*!
       * The qt settings key used to persistently store the chunk file extension value. 
       */
      static const char* _chunkExtensionKey;
      /*!
       * Points to the global singleton instance of this class. 
       */
      static Settings* _instance;
      Settings();
      void setValue(const QString& key, const QVariant& value);
      /*!
       * The platform index for the preferred OpenCL device. 
       */
      int _platform;
      /*!
       * The index for the preferred OpenCL device. 
       */
      int _device;
      /*!
       * The thread size used for accelerated analytic runs. 
       */
      int _threadSize;
      /*!
       * The buffer size used for the number of blocks each slave node is buffered with 
       * in MPI runs. 
       */
      int _bufferSize;
      /*!
       * The working directory for chunk runs. 
       */
      QString _chunkDir;
      /*!
       * The file name prefix for temporary chunk files. 
       */
      QString _chunkPrefix;
      /*!
       * The extension for temporary chunk files. 
       */
      QString _chunkExtension;
   };
}



#endif
