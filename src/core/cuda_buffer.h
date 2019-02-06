#ifndef CUDA_BUFFER_H
#define CUDA_BUFFER_H
#include "cuda_common.h"
#include "cuda_event.h"
#include "cuda_stream.h"



namespace CUDA
{
   /*!
    * This can contain an CUDA buffer object or be set to null. The buffer this
    * class contains can be moved to another instance but not copied. This is a
    * template class to enforce strong type checking of the underlying buffer data.
    *
    * @tparam T The type used to create the buffer data of this object.
    */
   template<class T> class Buffer
   {
   public:
      /*!
       * Constructs a new buffer object that is set to null.
       */
      Buffer() = default;
      Buffer(int size);
      Buffer(Buffer<T>&& other);
      ~Buffer();
      void operator=(Buffer<T>&& other);
      T& operator[](int index);
      const T& at(int index) const;
      bool isNull() const;
      CUdeviceptr deviceData() { return _dev; }
      T* hostData() { return _host; }
      int size() const { return _size; }
      Event read(const Stream& stream = Stream::getDefaultStream());
      Event write(const Stream& stream = Stream::getDefaultStream());
   private:
      void clear();
      void nullify();

      CUdeviceptr _dev {0};
      T* _host {nullptr};
      int _size {-1};
   };






   /*!
    * Constructs a new buffer object set to a new CUDA buffer with the given size.
    *
    * @param size The size of the buffer created as the number of elements of the
    *             template type defined.
    */
   template<class T> Buffer<T>::Buffer(int size):
      _size(size)
   {
      CUDA_SAFE_CALL(cuMemAllocHost(reinterpret_cast<void**>(&_host), size * sizeof(T)));
      CUDA_SAFE_CALL(cuMemAlloc(&_dev, size * sizeof(T)));
   }






   /*!
    * Constructs a new buffer object taking the state of the other given buffer
    * object.
    *
    * @param other The other buffer object whose state, null or set to an CUDA
    *              buffer, is taken by this new buffer object.
    */
   template<class T> Buffer<T>::Buffer(Buffer<T>&& other):
      _dev(other._dev),
      _host(other._host),
      _size(other._size)
   {
      other.nullify();
   }






   /*!
    * Clears all resources this buffer object may contain.
    */
   template<class T> Buffer<T>::~Buffer()
   {
      clear();
   }






   /*!
    * Takes the state of the given buffer object , null or set to a CUDA buffer,
    * overwriting and clearing any CUDA buffer this object may already contain.
    *
    * @param other The other buffer object whose state, null or set to an CUDA
    *              buffer, is taken by this buffer object.
    */
   template<class T> void Buffer<T>::operator=(Buffer<T>&& other)
   {
      clear();
      _dev = other._dev;
      _host = other._host;
      _size = other._size;
      other.nullify();
   }






   template<class T> T& Buffer<T>::operator[](int index)
   {
      if ( !_host )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Logic Error"));
         e.setDetails(QObject::tr("Cannot access data from unmapped CUDA buffer."));
         throw e;
      }
      if ( index < 0 || index >= _size )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Out Of Range"));
         e.setDetails(QObject::tr("The index %1 is out of range for this CUDA buffer (%2 size).")
                      .arg(index)
                      .arg(_size));
         throw e;
      }
      return _host[index];
   }






   template<class T> const T& Buffer<T>::at(int index) const
   {
      if ( !_host )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Logic Error"));
         e.setDetails(QObject::tr("Cannot access data from unmapped CUDA buffer."));
         throw e;
      }
      if ( index < 0 || index >= _size )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Out Of Range"));
         e.setDetails(QObject::tr("The index %1 is out of range for this CUDA buffer (%2 size).")
                      .arg(index)
                      .arg(_size));
         throw e;
      }
      return _host[index];
   }






   template<class T> bool Buffer<T>::isNull() const
   {
      return !_dev;
   }






   template<class T> Event Buffer<T>::read(const Stream& stream)
   {
      if ( !_dev )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Logic Error"));
         e.setDetails(QObject::tr("Cannot read from CUDA buffer that is null."));
         throw e;
      }

      // copy buffer from device to host
      CUDA_SAFE_CALL(cuMemcpyDtoHAsync(_host, _dev, _size * sizeof(T), stream.id()));

      // return CUDA event of memcpy
      Event event;
      event.record(stream);

      return event;
   }






   template<class T> Event Buffer<T>::write(const Stream& stream)
   {
      if ( !_dev )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Logic Error"));
         e.setDetails(QObject::tr("Cannot write to CUDA buffer that is null."));
         throw e;
      }

      // copy buffer from host to device
      CUDA_SAFE_CALL(cuMemcpyHtoDAsync(_dev, _host, _size * sizeof(T), stream.id()));

      // return CUDA event of memcpy
      Event event;
      event.record(stream);
      
      return event;
   }






   /*!
    * Release all memory resources. This does not set
    * any of this object's pointers to null.
    */
   template<class T> void Buffer<T>::clear()
   {
      CUDA_SAFE_CALL(cuMemFreeHost(_host));
      CUDA_SAFE_CALL(cuMemFree(_dev));
   }






   /*!
    * Sets this object to a null state by changing all its pointers to null. This does
    * not free any allocated memory those pointers may be pointing to.
    */
   template<class T> void Buffer<T>::nullify()
   {
      _dev = 0;
      _host = nullptr;
      _size = -1;
   }
}



#endif
