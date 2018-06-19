#ifndef OPENCL_KERNEL_LOCKER_H
#define OPENCL_KERNEL_LOCKER_H
#include "opencl_kernel.h"
//



namespace OpenCL
{
   /*!
    * This holds a lock on the given kernel object making it easy to manage the locked 
    * resource and avoid very hard to avoid deadlock bugs in complicated or exception 
    * handling code. This class can move its ownership of a kernel lock to another 
    * instance but it cannot be copied. Upon destruction this class calls the unlock 
    * method of the kernel object it contains. 
    */
   class Kernel::Locker
   {
   public:
      Locker(Kernel* kernel);
      Locker(Locker&& other);
      ~Locker();
   private:
      /*!
       * Pointer to the kernel object that is assumed to be locked and this object will 
       * unlock upon destruction. 
       */
      Kernel* _kernel;
   };
}



#endif
