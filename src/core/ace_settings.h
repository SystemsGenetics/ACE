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
    */
   class Settings
   {
   public:
      static QString organization();
      static QString application();
      static void initialize(QString organization, QString application);
      static Settings& instance();
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
       */
      constexpr static int _platformDefault {0};
      /*!
       */
      constexpr static int _deviceDefault {0};
      /*!
       */
      constexpr static int _threadSizeDefault {8};
      /*!
       */
      constexpr static int _bufferSizeDefault {4};
      /*!
       */
      static const char* _chunkDirDefault;
      /*!
       */
      static const char* _chunkPrefixDefault;
      /*!
       */
      static const char* _chunkExtensionDefault;
      /*!
       */
      static QString _organization;
      /*!
       */
      static QString _application;
      /*!
       */
      static const char* _platformKey;
      /*!
       */
      static const char* _deviceKey;
      /*!
       */
      static const char* _threadSizeKey;
      /*!
       */
      static const char* _bufferSizeKey;
      /*!
       */
      static const char* _chunkDirKey;
      /*!
       */
      static const char* _chunkPrefixKey;
      /*!
       */
      static const char* _chunkExtensionKey;
      /*!
       */
      static Settings* _instance;
      Settings();
      void setValue(const QString& key, const QVariant& value);
      /*!
       */
      int _platform;
      /*!
       */
      int _device;
      /*!
       */
      int _threadSize;
      /*!
       */
      int _bufferSize;
      /*!
       */
      QString _chunkDir {"."};
      /*!
       */
      QString _chunkPrefix {"chunk"};
      /*!
       */
      QString _chunkExtension {"abd"};
   };
}



#endif
