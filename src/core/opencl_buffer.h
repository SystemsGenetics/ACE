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
      Buffer() = default;
      Buffer(Context* context, int size);
      Buffer(Buffer<T>&& other);
      ~Buffer();
      void operator=(Buffer<T>&& other);
      T& operator[](int index);
      const T& at(int index) const;
      bool isNull() const;
      cl_mem id() const;
      int size() const;
      Event mapRead(CommandQueue* queue);
      Event mapWrite(CommandQueue* queue);
      Event unmap(CommandQueue* queue);
      T* data();
   private:
      Event map(CommandQueue* queue, cl_map_flags mapping);
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
      cl_command_queue _last;
      /*!
       */
      cl_map_flags _mapping;
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
      _last(other._last),
      _mapping(other._mapping)
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
      _mapping = other._mapping;
      other.nullify();
   }






   /*!
    *
    * @param index  
    */
   template<class T> T& Buffer<T>::operator[](int index)
   {
      if ( !_data )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Logic Error"));
         e.setDetails(QObject::tr("Cannot access data from unmapped OpenCL buffer."));
         throw e;
      }
      if ( _mapping != CL_MAP_WRITE )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Logic Error"));
         e.setDetails(QObject::tr("Cannot write data to OpenCL buffer mapped for reading."));
         throw e;
      }
      if ( index < 0 || index >= _size )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Out Of Range"));
         e.setDetails(QObject::tr("The index %1 is out of range for this OpenCL buffer (%2 size).")
                      .arg(index)
                      .arg(_size));
         throw e;
      }
      return _data[index];
   }






   /*!
    *
    * @param index  
    */
   template<class T> const T& Buffer<T>::at(int index) const
   {
      if ( !_data )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Logic Error"));
         e.setDetails(QObject::tr("Cannot access data from unmapped OpenCL buffer."));
         throw e;
      }
      if ( _mapping != CL_MAP_READ )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Logic Error"));
         e.setDetails(QObject::tr("Cannot read data from OpenCL buffer mapped for writing."));
         throw e;
      }
      if ( index < 0 || index >= _size )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Out Of Range"));
         e.setDetails(QObject::tr("The index %1 is out of range for this OpenCL buffer (%2 size).")
                      .arg(index)
                      .arg(_size));
         throw e;
      }
      return _data[index];
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
    */
   template<class T> Event Buffer<T>::mapRead(CommandQueue* queue)
   {
      return map(queue,CL_MAP_READ);
   }






   /*!
    *
    * @param queue  
    */
   template<class T> Event Buffer<T>::mapWrite(CommandQueue* queue)
   {
      return map(queue,CL_MAP_WRITE);
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
      cl_int code = {clReleaseCommandQueue(_last)};
      if ( code != CL_SUCCESS )
      {
         E_MAKE_EXCEPTION(e);
         fillException(&e,code);
         throw e;
      }
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
         e.setDetails(QObject::tr("Cannot access data from unmapped OpenCL buffer."));
         throw e;
      }
      return _data;
   }






   /*!
    *
    * @param queue  
    *
    * @param mapping  
    */
   template<class T> Event Buffer<T>::map(CommandQueue* queue, cl_map_flags mapping)
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
      cl_int code;
      cl_event id;
      _data = static_cast<T*>(clEnqueueMapBuffer(queue->id()
                                                 ,*_id
                                                 ,false
                                                 ,mapping
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
      _last = queue->id();
      code = clRetainCommandQueue(_last);
      if ( code != CL_SUCCESS )
      {
         E_MAKE_EXCEPTION(e);
         fillException(&e,code);
         throw e;
      }
      _mapping = mapping;
      return Event(id);
   }






   /*!
    */
   template<class T> void Buffer<T>::clear()
   {
      if ( _id )
      {
         if ( _data )
         {
            clEnqueueUnmapMemObject(_last,*_id,_data,0,nullptr,nullptr);
            clReleaseCommandQueue(_last);
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
