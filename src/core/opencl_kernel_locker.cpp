#include "opencl_kernel_locker.h"
#include "edebug.h"



using namespace OpenCL;
//






/*!
 * Constructs a new locker object with the given locked kernel object. 
 *
 * @param kernel  
 */
Kernel::Locker::Locker(Kernel* kernel):
   _kernel(kernel)
{
   EDEBUG_FUNC(this,kernel)
}






/*!
 * Constructs a new locker object taking ownership of the other locker object's 
 * kernel lock. 
 *
 * @param other  
 */
Kernel::Locker::Locker(Locker&& other):
   _kernel(other._kernel)
{
   EDEBUG_FUNC(this,&other)

   other._kernel = nullptr;
}






/*!
 * Unlocks the kernel object this locker object contains. 
 */
Kernel::Locker::~Locker()
{
   EDEBUG_FUNC(this)

   // If this object contains a kernel object then call its unlock method. 
   if ( _kernel )
   {
      _kernel->unlock();
   }
}
