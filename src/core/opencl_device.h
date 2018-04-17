#ifndef OPENCL_DEVICE_H
#define OPENCL_DEVICE_H
#include <CL/cl.h>
#include <QObject>
//



namespace OpenCL
{
   /*!
    */
   class Device : public QObject
   {
      Q_OBJECT
   public:
      /*!
       */
      enum class Type
      {
         /*!
          */
         CPU
         /*!
          */
         ,GPU
         /*!
          */
         ,Accelerator
         /*!
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
       */
      cl_device_id _id;
      /*!
       */
      bool _available;
      /*!
       */
      bool _compiler;
      /*!
       */
      QStringList _extensions;
      /*!
       */
      qint64 _globalMemorySize;
      /*!
       */
      qint64 _localMemorySize;
      /*!
       */
      int _computeUnitSize;
      /*!
       */
      int _clockFrequency;
      /*!
       */
      QString _name;
      /*!
       */
      Type _type;
   };
}



#endif
