#ifndef OPENCL_PLATFORM_H
#define OPENCL_PLATFORM_H
#include <CL/cl.h>
#include <QObject>
#include "opencl.h"
//



namespace OpenCL
{
   /*!
    * This provides information about a single OpenCL platform along with static 
    * methods for iterating through all available platforms. This also provides 
    * methods for iterating through all OpenCL devices of their respective platform. 
    * This class is not intended to be instantiated on its own but instead queried 
    * through the static methods. Any pointer to a platform the static methods return 
    * will exist for the lifetime of the ACE program. 
    */
   class Platform : public QObject
   {
      Q_OBJECT
   public:
      static int size();
      static OpenCL::Platform& get(int index);
      QString profile() const;
      QString version() const;
      QString name() const;
      QString vendor() const;
      const QStringList& extensions() const;
      int deviceSize() const;
      OpenCL::Device* device(int index) const;
   private:
      static void populate();
      explicit Platform(cl_platform_id id, QObject* parent = nullptr);
      void populateDevices();
      QString getInfo(cl_platform_info type) const;
      /*!
       * Global pointer list of OpenCL platform objects that is available on this system. 
       */
      static QList<OpenCL::Platform*>* _platforms;
      /*!
       * OpenCL platform ID for this platform. 
       */
      cl_platform_id _id;
      /*!
       * OpenCL profile for this platform. 
       */
      QString _profile;
      /*!
       * OpenCL version used by this platform. 
       */
      QString _version;
      /*!
       * Name of this platform. 
       */
      QString _name;
      /*!
       * Vendor of this OpenCL platform. 
       */
      QString _vendor;
      /*!
       * OpenCL extensions supported by this platform. 
       */
      QStringList _extensions;
      /*!
       * Pointer list of OpenCL devices this platform provides on this system. 
       */
      QList<OpenCL::Device*> _devices;
   };
}



#endif
