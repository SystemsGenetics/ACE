#ifndef CUDA_KERNEL_H
#define CUDA_KERNEL_H
#include "cuda_buffer.h"
#include "cuda_common.h"
#include "cuda_stream.h"



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
      CUfunction _kernel {nullptr};
      dim3 _gridDim {1};
      dim3 _blockDim {1};
      QVector<void*> _args;
      unsigned int _sharedMemBytes {0};
   };






   /*!
    * Set kernel argument.
    *
    * @param index
    * @param value
    */
   template<class T> void Kernel::setArgument(int index, T value)
   {
      // expand list to hold argument
      if ( _args.size() < index + 1 )
      {
         _args.resize(index + 1);
      }

      if ( _args[index] == nullptr )
      {
         _args[index] = new T;
      }

      // set kernel argument
      *(T*) _args[index] = value;
   }






   /*!
    * Set kernel buffer argument.
    *
    * @param index
    * @param buffer
    */
   template<class T> void Kernel::setBuffer(int index, Buffer<T>* buffer)
   {
      setArgument(index, buffer->deviceData());
   }
}



#endif
