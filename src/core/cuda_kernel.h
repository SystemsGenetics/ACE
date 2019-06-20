#ifndef CUDA_KERNEL_H
#define CUDA_KERNEL_H
#include "cuda_buffer.h"
#include "cuda_common.h"
#include "cuda_stream.h"
//



namespace CUDA
{
   /*!
    * This contains a CUDA kernel. This class is designed to be inherited and
    * implemented by specific kernel implementations. This design works to facilitate
    * separating the more arduous task of setting of a kernel to execute. This way
    * arguments for a kernel can be abstracted to C++ with easy to understand
    * arguments for methods. Because of this design most methods to set the kernel
    * arguments and work sizes are protected and should only be used by a child
    * inheriting this class.
    */
   class Kernel
   {
   public:
      Kernel(Program* program, const QString& name);
      virtual ~Kernel() {};
      Event execute(const Stream& stream = Stream::getDefaultStream());
   protected:
      int getAttribute(CUfunction_attribute attribute) const;
      void setSizes(dim3 globalSize, dim3 localSize);
      template<class T> void setArgument(int index, T value);
      template<class T> void setBuffer(int index, Buffer<T>* buffer);
      void setSharedMemory(unsigned int size) { _sharedMemBytes = size; }
   private:
      /*!
       * The CUDA kernel of this object.
       */
      CUfunction _kernel {nullptr};
      /*!
       * The global size for each dimension used by this kernel object for execution.
       */
      dim3 _gridDim {1};
      /*!
       * The local size for each dimension used by this kernel object for execution.
       */
      dim3 _blockDim {1};
      /*!
       * The list of arguments for this kernel object. Kernel arguments are
       * provided to the CUDA Driver API as a list of memory locations as void
       * pointers. Each memory location must contain either the literal value of
       * the argument or a device pointer if the argument is a device buffer.
       */
      QVector<void*> _args;
      /*!
       * The amount of shared memory (in bytes) to allocate dynamically when the
       * kernel is launched.
       */
      unsigned int _sharedMemBytes {0};
   };






   /*!
    * Sets this kernel's argument at the given index to the given value. The first 
    * argument of a kernel function is at index 0 and increments positively.
    *
    * @tparam T The type of the value that is set.
    *
    * @param index The kernel argument index whose value is set.
    *
    * @param value The value that the given kernel argument is set to.
    */
   template<class T> void Kernel::setArgument(int index, T value)
   {
      // Resize the argument list if necessary to contain the given index.
      if ( _args.size() < index + 1 )
      {
         _args.resize(index + 1);
      }

      // If the given argument index has not yet been set then allocate memory
      // for the argument value.
      if ( _args[index] == nullptr )
      {
         _args[index] = new T;
      }

      // Set the kernel argument with the given index to the given value.
      *reinterpret_cast<T*>(_args[index]) = value;
   }






   /*!
    * Sets this kernel's argument at the given index to the given CUDA buffer. The 
    * first argument of a kernel function is at index 0 and increments positively.
    *
    * @tparam T The buffer type that is set.
    *
    * @param index The kernel argument index whose value is set.
    *
    * @param buffer The buffer object that the given kernel argument is set to.
    */
   template<class T> void Kernel::setBuffer(int index, Buffer<T>* buffer)
   {
      // Set the kernel argument with the given index to the given CUDA buffer.
      setArgument(index, buffer->deviceData());
   }
}



#endif
