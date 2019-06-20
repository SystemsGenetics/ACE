#ifndef CUDA_BUFFER_H
#define CUDA_BUFFER_H
#include "cuda_common.h"
#include "cuda_event.h"
#include "cuda_stream.h"
//



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
      Buffer(int size, bool allocateHost = true);
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
      /*!
       * CUDA device pointer for this object. If this object is null then this 
       * is null. 
       */
      CUdeviceptr _dev {0};
      /*!
       * Host memory pointer to this object's mapped CUDA buffer. If this object
       * null then this is null. 
       */
      T* _host {nullptr};
      /*!
       * Size of this object's CUDA buffer as the number of elements of the defined 
       * template type. 
       */
      int _size {-1};
   };






   /*!
    * Constructs a new buffer object set to a new CUDA buffer with the given size.
    *
    * @param size The size of the buffer created as the number of elements of the
    *             template type defined.
    *
    * @param allocateHost Whether to allocate a host buffer in addition to a
    *                     device buffer.
    */
   template<class T> Buffer<T>::Buffer(int size, bool allocateHost):
      _size(size)
   {
      // Allocate a CUDA device buffer and a pinned host buffer.
      // If either allocation fails then throw an exception.
      if ( allocateHost )
      {
         CUresult result = cuMemAllocHost(reinterpret_cast<void**>(&_host), size * sizeof(T));
         if ( result != CUDA_SUCCESS )
         {
            E_MAKE_EXCEPTION(e);
            throwError(&e,result);
         }
      }
      CUresult result = cuMemAlloc(&_dev, size * sizeof(T));
      if ( result != CUDA_SUCCESS )
      {
         E_MAKE_EXCEPTION(e);
         throwError(&e,result);
      }
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
      // Clear this object of any CUDA resources, take the other object's state, and 
      // set the other object to null. 
      clear();
      _dev = other._dev;
      _host = other._host;
      _size = other._size;
      other.nullify();
   }






   /*!
    * Returns a reference to the element with the given index in this buffer object's 
    * host data for writing. If this buffer object does not contain host data, or
    * the index is out of range then an exception is thrown. 
    *
    * @param index The index of the element whose reference is returned. 
    *
    * @return Reference to element of mapped buffer with the given index. 
    */
   template<class T> T& Buffer<T>::operator[](int index)
   {
      // If this object is not mapped, or the given index is out of range then
      // throw an exception, else return a reference to the element with the given index. 
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






   /*!
    * Returns a read only reference to the element with the given index in this buffer 
    * object's host data for reading. If this buffer object does not contain host
    * data, or the index is out of range then an exception is thrown. 
    *
    * @param index  
    *
    * @return Read only reference to element of host buffer with the given index. 
    */
   template<class T> const T& Buffer<T>::at(int index) const
   {
      // If this object is not mapped, or the given index is out of range then
      // throw an exception, else return a read only reference to the element
      // with the given index. 
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






   /*!
    * Tests if this buffer object is null or contains a CUDA buffer. 
    *
    * @return True if this buffer object is null or false otherwise. 
    */
   template<class T> bool Buffer<T>::isNull() const
   {
      return !_dev;
   }






   /*!
    * Copies this object's CUDA buffer data to it's host memory pointer. The
    * command to copy memory is sent to the given CUDA stream and is not complete
    * until the returned event is complete. If this object is null then an exception
    * is thrown. 
    *
    * @param stream Pointer to the CUDA stream used to enqueue the CUDA command for 
    *               copying the memory. 
    *
    * @return The event for the copying command sent to the given CUDA stream. 
    */
   template<class T> Event Buffer<T>::read(const Stream& stream)
   {
      // If this object is null then throw an exception, else go to the next step. 
      if ( !_host || !_dev )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Logic Error"));
         e.setDetails(QObject::tr("Cannot read from CUDA buffer that is null."));
         throw e;
      }

      // Add a copy command to the given CUDA stream for this object's CUDA buffer.
      // If adding the command fails then throw an exception. 
      CUresult result = cuMemcpyDtoHAsync(_host, _dev, _size * sizeof(T), stream.id());
      if ( result != CUDA_SUCCESS )
      {
         E_MAKE_EXCEPTION(e);
         throwError(&e,result);
      }

      // Record the copy command to an event and return the event. Since the copy
      // command is the most recent command on the given stream, waiting for the
      // stream to complete its current commands is equivalent to waiting for the
      // copy command to complete.
      Event event;
      event.record(stream);

      return event;
   }






   /*!
    * Copies this object's host data to it's device memory pointer. The command
    * to copy memory is sent to the given CUDA stream and is not complete until
    * the returned event is complete. If this object is null then an exception
    * is thrown. 
    *
    * @param stream Pointer to the CUDA stream used to enqueue the CUDA command for 
    *               copying the memory. 
    *
    * @return The event for the copying command sent to the given CUDA stream. 
    */
   template<class T> Event Buffer<T>::write(const Stream& stream)
   {
      // If this object is null then throw an exception, else go to the next step. 
      if ( !_host || !_dev )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Logic Error"));
         e.setDetails(QObject::tr("Cannot write to CUDA buffer that is null."));
         throw e;
      }

      // Add a copy command to the given CUDA stream for this object's CUDA buffer.
      // If adding the command fails then throw an exception. 
      CUresult result = cuMemcpyHtoDAsync(_dev, _host, _size * sizeof(T), stream.id());
      if ( result != CUDA_SUCCESS )
      {
         E_MAKE_EXCEPTION(e);
         throwError(&e,result);
      }

      // Record the copy command to an event and return the event. Since the copy
      // command is the most recent command on the given stream, waiting for the
      // stream to complete its current commands is equivalent to waiting for the
      // copy command to complete.
      Event event;
      event.record(stream);
      
      return event;
   }






   /*!
    * Release all memory resources. This does not set any of this object's pointers to null.
    */
   template<class T> void Buffer<T>::clear()
   {
      // Free the device buffer and host buffer. If either command fails then
      // throw an exception.
      CUresult result = cuMemFreeHost(_host);
      if ( result != CUDA_SUCCESS )
      {
         E_MAKE_EXCEPTION(e);
         throwError(&e,result);
      }
      result = cuMemFree(_dev);
      if ( result != CUDA_SUCCESS )
      {
         E_MAKE_EXCEPTION(e);
         throwError(&e,result);
      }
   }






   /*!
    * Sets this object to a null state by changing all its pointers to null. This does
    * not free any allocated memory those pointers may be pointing to.
    */
   template<class T> void Buffer<T>::nullify()
   {
      // Set all this object's pointers and size to the null state. 
      _dev = 0;
      _host = nullptr;
      _size = -1;
   }
}



#endif
