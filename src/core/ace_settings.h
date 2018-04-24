#ifndef ACE_SETTINGS_H
#define ACE_SETTINGS_H
#include "opencl.h"
//



namespace Ace
{
   /*!
    */
   class Settings
   {
   public:
      static Settings& instance();
      OpenCL::Device* device() const;
      void setDevice(OpenCL::Device* device);
   private:
      Settings();
      /*!
       */
      static Settings* _instance;
      /*!
       */
      OpenCL::Device* _device;
   };
}



#endif
