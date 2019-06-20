#ifndef CUDA_DEVICE_H
#define CUDA_DEVICE_H
#include "cuda_common.h"



namespace CUDA
{
   /*!
    * This provides information about a single CUDA device along with static
    * methods for iterating through all available CUDA devices. This class is not
    * intended to be instantiated on its own but instead queried through the
    * static methods. Any device pointer returned by the statics will exist for
    * the lifetime of the ACE program.
    */
   class Device
   {
   public:
      static int size();
      static CUDA::Device* get(int index);
      Device(int ordinal);
      CUdevice id() const { return _id; }
      QString name() const;
      int getAttribute(CUdevice_attribute attribute) const;
      size_t getGlobalMemorySize() const;
   private:
      static void populate();
      /*!
       * Global pointer list of CUDA devices that are available on this system.
       */
      static QList<CUDA::Device*>* _devices;
      /*!
       * CUDA device ID for this device.
       */
      CUdevice _id;
   };
}



#endif
