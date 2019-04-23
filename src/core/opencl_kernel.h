#ifndef OPENCL_KERNEL_H
#define OPENCL_KERNEL_H
#include <CL/cl.h>
#include <QObject>
#include "opencl_buffer.h"
#include "opencl.h"
#include "edebug.h"



/*!
 * This contains all classes and functions associated with using OpenCL in ACE.
 */
namespace OpenCL
{
   /*!
    * This contains an OpenCL kernel. This class is designed to be inherited and
    * implemented by specific kernel implementations. This design works to
    * facilitate separating the more arduous task of setting of a kernel to
    * execute. This way arguments for a kernel can be abstracted to C++ with easy
    * to understand arguments for methods. Because of this design most methods to
    * set the kernel arguments and work sizes is protected and should only be used
    * by a child inheriting this class. Setting kernel parameters is also the only
    * part of OpenCL which is not thread safe. Because of this setting parameters
    * is protected behind a mutex and parameters can only be set by a thread when
    * it has locked that mutex.
    */
   class Kernel : public QObject
   {
      Q_OBJECT
   public:
      virtual ~Kernel() override;
   public:
      Event execute(CommandQueue* queue);
   protected:
      /*!
       * This holds a lock on the given kernel object making it easy to manage the
       * locked resource and avoid very hard to avoid deadlock bugs in complicated or
       * exception handling code. This class can move its ownership of a kernel lock
       * to another instance but it cannot be copied. Upon destruction this class
       * calls the unlock method of the kernel object it contains.
       */
      class Locker
      {
      public:
         Locker(Kernel* kernel);
         Locker(Locker&& other);
         ~Locker();
      private:
         /*!
          * Pointer to the kernel object that is assumed to be locked and this object
          * will unlock upon destruction.
          */
         Kernel* _kernel;
      };
   protected:
      explicit Kernel(Program* program, const QString& name, QObject* parent = nullptr);
   protected:
      Kernel::Locker lock();
      void unlock();
      int maxWorkGroupSize(Device* device) const;
      int workGroupMultiple(Device* device) const;
      void setDimensions(cl_uint size);
      void setSizes(cl_uint dimension, qint64 globalSize, qint64 localSize);
      template<class T> void setArgument(cl_uint index, T value);
      template<class T> void setBuffer(cl_uint index, Buffer<T>* buffer);
      template<class T> void setLocalMemory(cl_uint index, qint64 size);
   private:
      void allocate();
      void clear();
   private:
      /*!
       * The OpenCL kernel ID of this object.
       */
      cl_kernel _id;
      /*!
       * The number of dimensions this kernel object uses when executes.
       */
      cl_uint _size {1};
      /*!
       * The offsets for each dimension used by this kernel object for execution. The
       * offsets are always 0.
       */
      size_t* _offsets {nullptr};
      /*!
       * The global size for each dimension used by this kernel object for execution.
       * The default is 1.
       */
      size_t* _globalSizes {nullptr};
      /*!
       * The local or work group size for each dimension used by this kernel object
       * for execution. The default is 1.
       */
      size_t* _localSizes {nullptr};
      /*!
       * This protects this object's setting of parameters which is not thread safe in
       * OpenCL. Parameters can only be set when this is locked.
       */
      QMutex _lock;
      /*!
       * True if this kernel's mutex is locked for configuring this kernels parameters
       * or false otherwise.
       */
      bool _isLocked {false};
   };
}






namespace OpenCL
{
   /*!
    * Sets this kernel's argument at the given index to the given value. The first
    * argument of a kernel function is at index 0 and increments positively. If
    * this kernel is not locked then an exception is thrown.
    *
    * @tparam T The type of the value that is set.
    *
    * @param index The kernel argument index whose value is set.
    *
    * @param value The value that the given kernel argument is set to.
    */
   template<class T> void Kernel::setArgument(cl_uint index, T value)
   {
      // Add the debug header.
      EDEBUG_FUNC(this,index,value);
            
      // If this kernel is not locked then throw an exception, else go to the next step.
      if ( !_isLocked )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(tr("Logic Error"));
         e.setDetails(tr("Cannot set OpenCL kernel parameters without locking the object first."));
         throw e;
      }
            
      // Set the kernel argument with the given index to the given value. If setting the
      // argument fails then throw an exception.
      cl_int code {clSetKernelArg(_id,index,sizeof(T),&value)};
      if ( code != CL_SUCCESS )
      {
         E_MAKE_EXCEPTION(e);
         fillException(&e,code);
         throw e;
      }
   }






   /*!
    * Sets this kernel's argument at the given index to the given OpenCL buffer.
    * The first argument of a kernel function is at index 0 and increments
    * positively. If this kernel is not locked then an exception is thrown.
    *
    * @tparam T The buffer type that is set.
    *
    * @param index The kernel argument index whose value is set.
    *
    * @param buffer The buffer object that the given kernel argument is set to.
    */
   template<class T> void Kernel::setBuffer(cl_uint index, Buffer<T>* buffer)
   {
      // Add the debug header.
      EDEBUG_FUNC(this,index,buffer);
            
      // If this kernel is not locked then throw an exception, else go to the next step.
      if ( !_isLocked )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(tr("Logic Error"));
         e.setDetails(tr("Cannot set OpenCL kernel parameters without locking the object first."));
         throw e;
      }
            
      // Set the kernel argument with the given index to the given OpenCL buffer. If
      // setting the argument fails then throw an exception.
      cl_mem id {buffer->id()};
      cl_int code {clSetKernelArg(_id,index,sizeof(cl_mem),&id)};
      if ( code != CL_SUCCESS )
      {
         E_MAKE_EXCEPTION(e);
         fillException(&e,code);
         throw e;
      }
   }






   /*!
    * Sets this kernel's argument at the given index to local memory allocated to
    * the given size in elements of template type. The first argument of a kernel
    * function is at index 0 and increments positively. If this kernel is not
    * locked or the given size is less than one then an exception is thrown.
    *
    * @tparam T The element type whose given size in elements is allocated.
    *
    * @param index The kernel argument index whose value is set.
    *
    * @param size The number of elements of template type which is allocated in
    *             local memory.
    */
   template<class T> void Kernel::setLocalMemory(cl_uint index, qint64 size)
   {
      // Add the debug header.
      EDEBUG_FUNC(this,index,size);
            
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
         e.setDetails(tr("Cannot set local memory argument of OpenCL kernel with a size of less than 1."));
         throw e;
      }
            
      // Set the kernel argument with the given index to allocate the given amount of
      // local memory. If setting the argument fails then throw an exception.
      cl_int code {clSetKernelArg(_id,index,sizeof(T)*size,nullptr)};
      if ( code != CL_SUCCESS )
      {
         E_MAKE_EXCEPTION(e);
         fillException(&e,code);
         throw e;
      }
   }
}

#endif
