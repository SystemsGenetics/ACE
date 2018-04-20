#ifndef OPENCL_BUFFER_H
#define OPENCL_BUFFER_H
#include <CL/cl.h>
#include "opencl_context.h"
#include "opencl_commandqueue.h"
#include "opencl_event.h"
#include "opencl_common.h"
#include "eexception.h"
//



namespace OpenCL
{
   /*!
    *
    * @tparam T  
    */
   template<class T> class Buffer
   {
   public:
      /*!
       */
      enum Mapping
      {
         /*!
          */
         Read
         /*!
          */
         ,Write
      };
      /*!
       */
      Buffer() = default;
      Buffer(Context* context, int size);
      Buffer(Buffer<T>&& other);
      ~Buffer();
      void operator=(Buffer<T>&& other);
      bool isNull() const;
      cl_mem id() const;
      int size() const;
      Event map(CommandQueue* queue, Mapping mapping);
      Event unmap(CommandQueue* queue);
      T* data();
   private:
      void clear();
      void nullify();
      /*!
       */
      cl_mem* _id {nullptr};
      /*!
       */
      T* _data {nullptr};
      /*!
       */
      int _size {-1};
      /*!
       */
      CommandQueue* _last;
   };






   /*!
    *
    * @param context  
    *
    * @param size  
    */
   template<class T> Buffer<T>::Buffer(Context* context, int size):
      _id(new cl_mem),
      _size(size)
   {
      cl_int code;
      *_id = clCreateBuffer(context->id(),CL_MEM_READ_WRITE,sizeof(T)*_size,nullptr,&code);
      if ( code != CL_SUCCESS )
      {
         E_MAKE_EXCEPTION(e);
         fillException(&e,code);
         throw e;
      }
   }






   /*!
    *
    * @param other  
    */
   template<class T> Buffer<T>::Buffer(Buffer<T>&& other):
      _id(other._id),
      _data(other._data),
      _size(other._size),
      _last(other._last)
   {
      other.nullify();
   }






   /*!
    */
   template<class T> Buffer<T>::~Buffer()
   {
      clear();
   }






   /*!
    *
    * @param other  
    */
   template<class T> void Buffer<T>::operator=(Buffer<T>&& other)
   {
      clear();
      _id = other._id;
      _data = other._data;
      _size = other._size;
      _last = other._last;
      other.nullify();
   }






   /*!
    */
   template<class T> bool Buffer<T>::isNull() const
   {
      return !_id;
   }






   /*!
    */
   template<class T> cl_mem Buffer<T>::id() const
   {
      if ( !_id )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Logic Error"));
         e.setDetails(QObject::tr("Cannot return OpenCL buffer ID of null object."));
         throw e;
      }
      return *_id;
   }






   /*!
    */
   template<class T> int Buffer<T>::size() const
   {
      return _size;
   }






   /*!
    *
    * @param queue  
    *
    * @param mapping  
    */
   template<class T> Event Buffer<T>::map(CommandQueue* queue, Mapping mapping)
   {
      if ( !_id )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Logic Error"));
         e.setDetails(QObject::tr("Cannot unmap OpenCL buffer that is null."));
         throw e;
      }
      if ( _data )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Logic Error"));
         e.setDetails(QObject::tr("Cannot map OpenCL buffer that is already mapped."));
         throw e;
      }
      cl_map_flags flags;
      switch (mapping)
      {
      case Read:
         flags = CL_MAP_READ;
         break;
      case Write:
         flags = CL_MAP_WRITE;
         break;
      }
      cl_int code;
      cl_event id;
      _data = static_cast<T*>(clEnqueueMapBuffer(queue->id()
                                                 ,*_id
                                                 ,false
                                                 ,flags
                                                 ,0
                                                 ,sizeof(T)*_size
                                                 ,0
                                                 ,nullptr
                                                 ,&id
                                                 ,&code));
      if ( code != CL_SUCCESS )
      {
         E_MAKE_EXCEPTION(e);
         fillException(&e,code);
         throw e;
      }
      _last = queue;
      return Event(id);
   }






   /*!
    *
    * @param queue  
    */
   template<class T> Event Buffer<T>::unmap(CommandQueue* queue)
   {
      if ( !_id )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Logic Error"));
         e.setDetails(QObject::tr("Cannot unmap OpenCL buffer that is null."));
         throw e;
      }
      if ( !_data )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Logic Error"));
         e.setDetails(QObject::tr("Cannot unmap OpenCL buffer that is not mapped."));
         throw e;
      }
      cl_event id;
      clEnqueueUnmapMemObject(queue->id(),*_id,_data,0,nullptr,&id);
      _data = nullptr;
      return Event(id);
   }






   /*!
    */
   template<class T> T* Buffer<T>::data()
   {
      if ( !_data )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Logic Error"));
         e.setDetails(QObject::tr("Cannot return mapped memory pointer from unmapped OpenCL buffer."));
         throw e;
      }
      return _data;
   }






   /*!
    */
   template<class T> void Buffer<T>::clear()
   {
      if ( _id )
      {
         if ( _data )
         {
            clEnqueueUnmapMemObject(_last->id(),*_id,_data,0,nullptr,nullptr);
         }
         clReleaseMemObject(*_id);
         delete _id;
      }
   }






   /*!
    */
   template<class T> void Buffer<T>::nullify()
   {
      _id = nullptr;
      _data = nullptr;
      _size = -1;
      _last = nullptr;
   }
}



#endif
