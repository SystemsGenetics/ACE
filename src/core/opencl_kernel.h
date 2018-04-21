#ifndef OPENCL_KERNEL_H
#define OPENCL_KERNEL_H
#include <CL/cl.h>
#include <QObject>
#include "opencl_buffer.h"
#include "opencl.h"
//



namespace OpenCL
{
   /*!
    */
   class Kernel : public QObject
   {
      Q_OBJECT
   public:
      virtual ~Kernel() override;
      Event execute(CommandQueue* queue);
   protected:
      explicit Kernel(Program* program, const QString& name, QObject* parent = nullptr);
      int maxWorkGroupSize(Device* device) const;
      int workGroupMultiple(Device* device) const;
      void setDimensions(cl_uint size);
      void setSizes(cl_uint dimension, qint64 globalSize, qint64 localSize);
      template<class T> void setArgument(cl_uint index, T value);
      template<class T> void setBuffer(cl_uint index, const Buffer<T>& buffer);
      template<class T> void setLocalMemory(cl_uint index, qint64 size);
   private:
      void resize();
      void clear();
      /*!
       */
      cl_kernel _id;
      /*!
       */
      cl_uint _size {0};
      /*!
       */
      size_t* _offsets {nullptr};
      /*!
       */
      size_t* _globalSizes {nullptr};
      /*!
       */
      size_t* _localSizes {nullptr};
   };






   /*!
    *
    * @tparam T  
    *
    * @param index  
    *
    * @param value  
    */
   template<class T> void Kernel::setArgument(cl_uint index, T value)
   {
      cl_int code {clSetKernelArg(_id,index,sizeof(T),&value)};
      if ( code != CL_SUCCESS )
      {
         E_MAKE_EXCEPTION(e);
         fillException(&e,code);
         throw e;
      }
   }






   /*!
    *
    * @tparam T  
    *
    * @param index  
    *
    * @param buffer  
    */
   template<class T> void Kernel::setBuffer(cl_uint index, const Buffer<T>& buffer)
   {
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
    *
    * @tparam T  
    *
    * @param index  
    *
    * @param size  
    */
   template<class T> void Kernel::setLocalMemory(cl_uint index, qint64 size)
   {
      if ( size < 1 )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(tr("Invalid Argument"));
         e.setDetails(tr("Cannot set local memory argument of OpenCL kernel with a size of less than 1."));
         throw e;
      }
      cl_int code {clSetKernelArg(_id,index,size,nullptr)};
      if ( code != CL_SUCCESS )
      {
         E_MAKE_EXCEPTION(e);
         fillException(&e,code);
         throw e;
      }
   }
}



#endif
