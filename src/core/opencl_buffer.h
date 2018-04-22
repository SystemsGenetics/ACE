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
    * This can contain an OpenCL buffer object or be set to null. The buffer this 
    * class contains can be moved to another instance but not copied. This is a 
    * template class to enforce strong type checking of the underlying buffer data. 
    * This class provides methods for mapping the OpenCL buffer memory to a temporary 
    * host pointer and can be accessed through other helper methods that provides 
    * range checking or through a raw memory pointer. Other methods are provided for 
    * getting the size of the buffer and the underlying OpenCL memory ID used by other 
    * OpenCL classes. 
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
       * Pointer to the OpenCL buffer ID of this object. If this object is null then this 
       * is null. 
       */
      cl_mem* _id {nullptr};
      /*!
       * Host memory pointer to this object's mapped OpenCL buffer. If this object is not 
       * mapped then this is null. 
       */
      T* _data {nullptr};
      /*!
       * Size of this object's OpenCL buffer as the number of elements of the defined 
       * template type. 
       */
      int _size {-1};
      /*!
       * OpenCL command queue ID of the last command queue to be used for mapping this 
       * object's buffer. This is used to unmap the buffer if this object is destroyed. 
       */
      cl_command_queue _last;
      /*!
       * The type of mapping of this object's OpenCL buffer if it is mapped. If this 
       * object is not mapped this is not used and undefined. 
       */
      cl_map_flags _mapping;
   };






   /*!
    * Constructs a new buffer object set to a new OpenCL buffer with the given context 
    * and size. 
    *
    * @param context Pointer to context that this buffer object is created from. 
    *
    * @param size The size of the buffer created as the number of elements of the 
    *             template type defined. 
    *
    *
    * Steps of Operation: 
    *
    * 1. Create a new OpenCL buffer and set this object's buffer ID to the new buffer. 
    *    If creation fails then throw an exception. 
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
    * Constructs a new buffer object taking the state of the other given buffer 
    * object. 
    *
    * @param other The other buffer object whose state, null or set to an OpenCL 
    *              buffer, is taken by this new buffer object. 
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
    * Clears all resources this buffer object may contain. 
    */
   template<class T> Buffer<T>::~Buffer()
   {
      clear();
   }






   /*!
    * Takes the state of the given buffer object , null or set to an OpenCL buffer, 
    * overwriting and clearing any OpenCL buffer this object may already contain. 
    *
    * @param other The other buffer object whose state, null or set to an OpenCL 
    *              buffer, is taken by this buffer object. 
    *
    *
    * Steps of Operation: 
    *
    * 1. Clear this object of any OpenCL resources, take the other object's state, and 
    *    set the other object to null. 
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
    * Returns a reference to the element with the given index in this buffer object's 
    * mapped data for writing. If this buffer object is not mapped, the mapping is not 
    * for writing, or the index is out of range then an exception is thrown. 
    *
    * @param index The index of the element whose reference is returned. 
    *
    * @return Reference to element of mapped buffer with the given index. 
    *
    *
    * Steps of Operation: 
    *
    * 1. If this object is not mapped, the mapping is not for writing, or the given 
    *    index is out of range then throw an exception, else return a reference to the 
    *    mapped element with the given index. 
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
    * Returns a read only reference to the element with the given index in this buffer 
    * object's mapped data for reading. If this buffer object is not mapped, the 
    * mapping is not for reading, or the index is out of range then an exception is 
    * thrown. 
    *
    * @param index  
    *
    * @return Read only reference to element of mapped buffer with the given index. 
    *
    *
    * Steps of Operation: 
    *
    * 1. If this object is not mapped, the mapping is not for reading, or the given 
    *    index is out of range then throw an exception, else return a read only 
    *    reference to the mapped element with the given index. 
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
    * Tests if this buffer object is null or contains an OpenCL buffer. 
    *
    * @return True if this buffer object is null or false otherwise. 
    */
   template<class T> bool Buffer<T>::isNull() const
   {
      return !_id;
   }






   /*!
    * Returns the OpenCL buffer ID of this object. If this object is null then an 
    * exception is thrown. 
    *
    * @return OpenCL buffer ID of this object. 
    *
    *
    * Steps of Operation: 
    *
    * 1. If this object is null then throw an exception, else return this object's 
    *    OpenCL buffer ID. 
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
    * Returns the size of this object's OpenCL buffer as the number of elements of the 
    * template type defined. If this object is null then -1 is returned. 
    *
    * @return The number of elements this buffer object contains or -1 if this object 
    *         is null. 
    */
   template<class T> int Buffer<T>::size() const
   {
      return _size;
   }






   /*!
    * Maps this object's OpenCL buffer data to a host memory pointer for reading that 
    * can be accessed through other methods of this class. The command to map memory 
    * is sent to the given command queue and is not complete until the returned event 
    * is complete. If this object is null or is already mapped then an exception is 
    * thrown. 
    *
    * @param queue Pointer to the command queue used to add the OpenCL command for 
    *              mapping the memory. 
    *
    * @return The event for the mapping command sent to the given command queue. 
    */
   template<class T> Event Buffer<T>::mapRead(CommandQueue* queue)
   {
      return map(queue,CL_MAP_READ);
   }






   /*!
    * Maps this object's OpenCL buffer data to a host memory pointer for writing that 
    * can be accessed through other methods of this class. The command to map memory 
    * is sent to the given command queue and is not complete until the returned event 
    * is complete. If this object is null or is already mapped then an exception is 
    * thrown. 
    *
    * @param queue Pointer to the command queue used to add the OpenCL command for 
    *              mapping the memory. 
    *
    * @return The event for the mapping command sent to the given command queue. 
    */
   template<class T> Event Buffer<T>::mapWrite(CommandQueue* queue)
   {
      return map(queue,CL_MAP_WRITE);
   }






   /*!
    * Unmaps this object's OpenCL buffer data from host memory. The command to unmap 
    * memory is sent to the given command queue and is not complete until the returned 
    * event is complete. If the object is null or is not mapped then an exception is 
    * thrown. 
    *
    * @param queue Pointer to the command queue used to add the OpenCL command for 
    *              unmapping the memory. 
    *
    * @return The event for the unmapping command sent to the given command queue. 
    *
    *
    * Steps of Operation: 
    *
    * 1. If this object is null or is not mapped then throw an exception, else go to 
    *    the next step. 
    *
    * 2. Add an unmap command to the given command queue for this object's OpenCL 
    *    buffer. If adding the command fails then throw an exception. 
    *
    * 3. Release the OpenCL command queue used to map this object's buffer and return 
    *    the event of the unmapping command added to the given command queue in the 
    *    last step. If releasing the command queue fails then throw an exception. 
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
    * Returns the host memory pointer of this object's mapped OpenCL buffer data. If 
    * this object's buffer is not mapped then an exception is thrown. Care must be 
    * taken to only read or write to this pointer depending on what type of mapping it 
    * is. Reading from a write mapping or writing to a read mapping is undefined. 
    *
    * @return Pointer to host memory mapping of this object's OpenCL buffer. 
    *
    *
    * Steps of Operation: 
    *
    * 1. If this object is not mapped then throw an exception, else return the host 
    *    memory pointer of this object's mapped OpenCL buffer. 
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
    * Maps this object's OpenCL buffer to host memory using the given command queue to 
    * add the command and the given mapping to set it for reading or writing. If this 
    * object is null or is already mapped then an exception is thrown. 
    *
    * @param queue Pointer to the command queue used to add the mapping command. 
    *
    * @param mapping The type of mapping used, either read or write. 
    *
    * @return The event for the mapping command added to the given command queue. 
    *
    *
    * Steps of Operation: 
    *
    * 1. If this object is null or is already mapped then throw an exception, else go 
    *    to the next step. 
    *
    * 2. Add a map command to the given command queue for this object's OpenCL buffer 
    *    with the given mapping. If adding the command fails then throw an exception. 
    *
    * 3. Save the given command queue ID to this object, retain it, and return the 
    *    event of the mapping command added to the given command queue in the last 
    *    step. If retaining the command queue fails then throw an exception. 
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
    * Clears any mapping of this object's OpenCL buffer if it has one, releases all 
    * OpenCL resources, and deletes the OpenCL memory ID variable. This does not set 
    * any of this object's pointers to null. 
    *
    *
    * Steps of Operation: 
    *
    * 1. If this object is null then do nothing and exit, else go to the next step. 
    *
    * 2. If this object is mapped then call an unmap command with this object's saved 
    *    command queue and then release it. 
    *
    * 3. Release this object's OpenCL buffer ID and then delete it. 
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
    * Sets this object to a null state by changing all its pointers to null. This does 
    * not free any allocated memory those pointers may be pointing to. 
    *
    *
    * Steps of Operation: 
    *
    * 1. Set all this object's pointers and size to the null state. 
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
