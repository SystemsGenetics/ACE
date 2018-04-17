#ifndef OPENCL_PLATFORM_H
#define OPENCL_PLATFORM_H
#include <CL/cl.h>
#include <QObject>
#include "opencl.h"
//



namespace OpenCL
{
   /*!
    */
   class Platform : public QObject
   {
      Q_OBJECT
   public:
      static int size();
      static OpenCL::Platform* get(int index);
      explicit Platform(cl_platform_id id, QObject* parent = nullptr);
      QString profile() const;
      QString version() const;
      QString name() const;
      QString vendor() const;
      const QStringList& extensions() const;
      int deviceSize() const;
      OpenCL::Device* device(int index) const;
   private:
      static void populate();
      void populateDevices();
      QString getInfo(cl_platform_info type) const;
      /*!
       */
      static QList<OpenCL::Platform*>* _platforms;
      /*!
       */
      cl_platform_id _id;
      /*!
       */
      QString _profile;
      /*!
       */
      QString _version;
      /*!
       */
      QString _name;
      /*!
       */
      QString _vendor;
      /*!
       */
      QStringList _extensions;
      /*!
       */
      QList<OpenCL::Device*> _devices;
   };
}



#endif
