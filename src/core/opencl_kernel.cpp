#include "opencl_kernel.h"
#include "opencl_kernel_locker.h"
#include "opencl_device.h"
#include "opencl_program.h"
#include "opencl_commandqueue.h"
#include "opencl_event.h"
#include "opencl_common.h"
#include "ace_settings.h"
#include "eexception.h"



using namespace OpenCL;
//






/*!
 * Clears all resources this kernel contains. 
 */
Kernel::~Kernel()
{
   // Release the OpenCL kernel and clear all arrays. 
   clReleaseKernel(_id);
   clear();
}






/*!
 * Executes this object's OpenCL kernel on the given command queue with the 
 * dimensions, global, and local sizes this object possesses, returning the event 
 * for the kernel command. 
 *
 * @param queue  
 *
 * @return The event for the kernel command running on the given command queue. 
 */
Event Kernel::execute(CommandQueue* queue)
{
   // Add a ND range kernel execution command to the given command queue with this 
   // objects dimensions of offsets, global sizes, and local sizes, returning the 
   // event for the added command. If adding the command fails then throw an 
   // exception. 
   cl_event id;
   cl_int code
   {
      clEnqueueNDRangeKernel(queue->id(),_id,_size,_offsets,_globalSizes,_localSizes,0,nullptr,&id)
   };
   if ( code != CL_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      fillException(&e,code);
      throw e;
   }
   return Event(id);
}






/*!
 * Constructs a new kernel object from the given program with the given kernel name 
 * and optional parent. 
 *
 * @param program Pointer to the program which has built the kernel with the given 
 *                name that is created. 
 *
 * @param name The name of the kernel that is created. 
 *
 * @param parent Optional parent for this new kernel object. 
 */
Kernel::Kernel(Program* program, const QString& name, QObject* parent):
   QObject(parent)
{
   // Create a new OpenCL kernel from the given program with the given name, storing 
   // its id to this object. If creating the kernel fails then throw an exception. 
   cl_int code;
   _id = clCreateKernel(program->id(),name.toLocal8Bit().data(),&code);
   if ( code != CL_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      fillException(&e,code);
      throw e;
   }

   // Allocate all dimension arrays. 
   allocate();
}






/*!
 * Locks this kernel object allowing the setting of this object's kernel parameters 
 * and returning a locker object that unlocks this kernel on its destruction. 
 *
 * @return Locker object that unlocks this kernel upon its destruction. 
 */
Kernel::Locker Kernel::lock()
{
   // Lock this kernel's mutex, set this object as locked, and return a locker object 
   // pointing to this kernel. 
   _lock.lock();
   _isLocked = true;
   return Locker(this);
}






/*!
 * Unlocks this kernel object after which parameters cannot be set and allows other 
 * threads to lock it. 
 */
void Kernel::unlock()
{
   // Unlock this kernel's mutex and set this object as unlocked. 
   _lock.unlock();
   _isLocked = false;
}






/*!
 * Returns the maximum work group (local) size this kernel can possess for the 
 * given device. If this kernel is not locked then an exception is thrown. 
 *
 * @param device  
 *
 * @return Maximum work group size this kernel can possess for the given device. 
 */
int Kernel::maxWorkGroupSize(Device* device) const
{
   // If this kernel is not locked then throw an exception, else go to the next step. 
   if ( !_isLocked )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Cannot set OpenCL kernel parameters without locking the object first."));
      throw e;
   }

   // Get the maximum work group size for this kernel if ran on the given device and 
   // return it. If getting that information fails then throw an exception. 
   size_t size;
   cl_int code
   {
      clGetKernelWorkGroupInfo(_id
                               ,device->id()
                               ,CL_KERNEL_WORK_GROUP_SIZE
                               ,sizeof(size_t)
                               ,&size
                               ,nullptr)
   };
   if ( code != CL_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      fillException(&e,code);
      throw e;
   }
   return size;
}






/*!
 * Returns the recommended, for efficiency, work group multiple for the given 
 * device. If this kernel is not locked then an exception is thrown. 
 *
 * @param device  
 *
 * @return Work group multiple for the given device. 
 */
int Kernel::workGroupMultiple(Device* device) const
{
   // If this kernel is not locked then throw an exception, else go to the next step. 
   if ( !_isLocked )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Cannot set OpenCL kernel parameters without locking the object first."));
      throw e;
   }

   // Get the work group multiple for the given device and return it. If getting that 
   // information fails then throw an exception. 
   size_t size;
   cl_int code
   {
      clGetKernelWorkGroupInfo(_id
                               ,device->id()
                               ,CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE
                               ,sizeof(size_t)
                               ,&size
                               ,nullptr)
   };
   if ( code != CL_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      fillException(&e,code);
      throw e;
   }
   return size;
}






/*!
 * Sets the number of dimensions for parallel execution of this kernel object. If 
 * this kernel is not locked or the given size is less than one then an exception 
 * is thrown. 
 *
 * @param size The number of dimensions for this kernel. 
 */
void Kernel::setDimensions(cl_uint size)
{
   // If this kernel is not locked or the given size is less than one then throw an 
   // exception, else go to the next step. 
   if ( !_isLocked )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Cannot set OpenCL kernel parameters without locking the object first."));
      throw e;
   }
   if ( size < 1 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("%1 is not a valid dimension size for an OpenCL Kernel (must be 1 or geater).")
                   .arg(size));
      throw e;
   }

   // if the new given size is different form this object's current dimension size 
   // then change it to the new size, clear the previous arrays, and allocate new 
   // arrays. 
   if ( size != _size )
   {
      _size = size;
      clear();
      allocate();
   }
}






/*!
 * Sets the global and local sizes of the given dimension used for parallel 
 * execution of this kernel object. If this kernel is not locked, the dimension is 
 * invalid, the local or group size is less than one, or the global size is not a 
 * multiple of the local size then an exception is thrown. 
 *
 * @param dimension The dimension whose global and local sizes are set. 
 *
 * @param globalSize The new global size that is set. This must be a multiple of 
 *                   the local size. 
 *
 * @param localSize The new local or work group size that is set. This must be 
 *                  divisible of the global size. 
 */
void Kernel::setSizes(cl_uint dimension, qint64 globalSize, qint64 localSize)
{
   // If this kernel is not locked, the given dimension is out of rage, the global or 
   // local sizes are less then one, or the global size is not a multiple of the 
   // local size then throw an exception, else go to the next step. 
   if ( !_isLocked )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Cannot set OpenCL kernel parameters without locking the object first."));
      throw e;
   }
   if ( dimension >= _size )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Out Of Range"));
      e.setDetails(tr("Cannot set dimension %1 sizes of OpenCL kernel (%2 total).")
                   .arg(dimension)
                   .arg(_size));
      throw e;
   }
   if ( globalSize < 1 || localSize < 1 || globalSize%localSize )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("%1 and %2 are not valid global and local OpenCL kernel sizes, respectively.")
                   .arg(globalSize)
                   .arg(localSize));
      throw e;
   }

   // Set the global and local sizes of the given dimension to the given values. 
   _globalSizes[dimension] = globalSize;
   _localSizes[dimension] = localSize;
}






/*!
 * Allocates new arrays for the offsets, global sizes, and local sizes of this 
 * object used for adding a kernel parallel execution command. The dimension size 
 * is used for the new sizes. Any memory pointed to previously is overwritten and 
 * not deleted. 
 */
void Kernel::allocate()
{
   // Allocate new arrays for offsets, global sizes, and local sizes, setting this 
   // object's pointers to the new arrays. 
   _offsets = new size_t[_size];
   _globalSizes = new size_t[_size];
   _localSizes = new size_t[_size];

   // Iterate through all new arrays and set their offsets to 0, global size to 1, 
   // and local size to 1. 
   for (cl_uint i = 0; i < _size ;++i)
   {
      _offsets[i] = 0;
      _globalSizes[i] = 1;
      _localSizes[i] = 1;
   }
}






/*!
 * Deletes arrays pointed to by this object's offsets, global sizes, and local 
 * sizes array pointers. 
 */
void Kernel::clear()
{
   // Delete all arrays this object points to. 
   delete[] _offsets;
   delete[] _globalSizes;
   delete[] _localSizes;
}
