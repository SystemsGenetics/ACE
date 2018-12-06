#ifndef CUDA_DEVICE_H
#define CUDA_DEVICE_H
#include "cuda_common.h"



namespace CUDA
{
   /*!
    * This provides information about a single CUDA device.
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
      static QList<CUDA::Device*>* _devices;
      CUdevice _id;
   };
}



#endif
