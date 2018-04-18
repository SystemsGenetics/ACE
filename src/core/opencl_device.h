#ifndef OPENCL_DEVICE_H
#define OPENCL_DEVICE_H
#include <CL/cl.h>
#include <QObject>
//



namespace OpenCL
{
   /*!
    * This provides information about a single OpenCL device. This class is not 
    * intended to be instantiated on its own but instead queried through the platform 
    * class. Any pointer to a device the platform methods return will exist for the 
    * life time of the ACE program. This class also provides its underlying OpenCL 
    * device ID that is used by other OpenCL classes. 
    */
   class Device : public QObject
   {
      Q_OBJECT
   public:
      /*!
       * Defines the different OpenCL device types a device can be. 
       */
      enum class Type
      {
         /*!
          * Defines the CPU device type. 
          */
         CPU
         /*!
          * Defines the GPU device type. 
          */
         ,GPU
         /*!
          * Defines the accelerator device type. 
          */
         ,Accelerator
         /*!
          * Defines the default device type. 
          */
         ,Default
      };
      explicit Device(cl_device_id id, QObject* parent = nullptr);
      cl_device_id id() const;
      bool isAvailable() const;
      bool hasCompiler() const;
      const QStringList& extensions() const;
      qint64 globalMemorySize() const;
      qint64 localMemorySize() const;
      int computeUnitSize() const;
      int clockFrequency() const;
      QString name() const;
      OpenCL::Device::Type type() const;
   private:
      template<class R,class T> R getInfo(cl_device_info type) const;
      QString getStringInfo(cl_device_info type) const;
      /*!
       * OpenCL device ID of this device. 
       */
      cl_device_id _id;
      /*!
       * True if this device is available to use or false otherwise. 
       */
      bool _available;
      /*!
       * True if this device can compile OpenCL kernel code or false otherwise. 
       */
      bool _compiler;
      /*!
       * OpenCL extensions supported by this device. 
       */
      QStringList _extensions;
      /*!
       * Global memory size of this device in bytes. 
       */
      qint64 _globalMemorySize;
      /*!
       * Local memory size of this device in bytes. 
       */
      qint64 _localMemorySize;
      /*!
       * Maximum number of compute units this device's hardware can use at any one point 
       * in parallel. 
       */
      int _computeUnitSize;
      /*!
       * Maximum clock frequency of this device's hardware in megahertz. 
       */
      int _clockFrequency;
      /*!
       * Name of this device. 
       */
      QString _name;
      /*!
       * OpenCL device type of this device. 
       */
      Type _type;
   };
}



#endif
