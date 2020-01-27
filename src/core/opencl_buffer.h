#ifndef OPENCL_BUFFER_H
#define OPENCL_BUFFER_H
#include <CL/cl.h>
#include "opencl_context.h"
#include "opencl_commandqueue.h"
#include "opencl_event.h"
#include "opencl.h"
#include "eexception.h"
#include "edebug.h"

namespace OpenCL
{




/*!
 * This can contain an OpenCL buffer object or be set to null. The buffer this class contains can be
 * moved to another instance but not copied. This is a template class to enforce strong type
 * checking of the underlying buffer data. This class provides methods for mapping the OpenCL buffer
 * memory to a temporary host pointer and can be accessed through other helper methods that provides
 * range checking or through a raw memory pointer. Other methods are provided for getting the size
 * of the buffer and the underlying OpenCL memory ID used by other OpenCL classes.
 *
 * @tparam T : The type used to create the buffer data of this object.
 */
template<class T>
class Buffer
{


/****************/
/* Constructors */
/****************/
public:


    /*!
     * Constructs a new buffer object that is set to null.
     */
    Buffer() = default;


    /*!
     * Constructs a new buffer object set to a new OpenCL buffer with the given context and size.
     *
     * @param context : Pointer to context that this buffer object is created from.
     *
     * @param size : The size of the buffer created as the number of elements of the template type
     *               defined.
     */
    Buffer(
        Context* context
        ,int size
    );


    /*!
     * Constructs a new buffer object taking the state of the other given buffer object.
     *
     * @param other : The other buffer object whose state, null or set to an OpenCL buffer, is taken
     *                by this new buffer object.
     */
    Buffer(
        Buffer<T>&& other
    );


/**************/
/* Destructor */
/**************/
public:


    /*!
     * Clears all resources this buffer object may contain.
     */
    ~Buffer();


/*************/
/* Operators */
/*************/
public:


    /*!
     * Takes the state of the given buffer object , null or set to an OpenCL buffer, overwriting and
     * clearing any OpenCL buffer this object may already contain.
     *
     * @param other : The other buffer object whose state, null or set to an OpenCL buffer, is taken
     *                by this buffer object.
     */
    void operator=(
        Buffer<T>&& other
    );


    /*!
     * Returns a reference to the element with the given index in this buffer object's mapped data
     * for writing. If this buffer object is not mapped, the mapping is not for writing, or the
     * index is out of range then an exception is thrown.
     *
     * @param index : The index of the element whose reference is returned.
     *
     * @return : Reference to element of mapped buffer with the given index.
     */
    T& operator[](
        int index
    );


/***********/
/* Methods */
/***********/
public:


    /*!
     * Returns a read only reference to the element with the given index in this buffer object's
     * mapped data for reading. If this buffer object is not mapped, the mapping is not for reading,
     * or the index is out of range then an exception is thrown.
     *
     * @param index : Detailed description.
     *
     * @return : Read only reference to element of mapped buffer with the given index.
     */
    const T& at(
        int index
    ) const;


    /*!
     * Tests if this buffer object is null or contains an OpenCL buffer.
     *
     * @return : True if this buffer object is null or false otherwise.
     */
    bool isNull() const;


    /*!
     * Returns the OpenCL buffer ID of this object. If this object is null then an exception is
     * thrown.
     *
     * @return : OpenCL buffer ID of this object.
     */
    cl_mem id() const;


    /*!
     * Returns the size of this object's OpenCL buffer as the number of elements of the template
     * type defined. If this object is null then -1 is returned.
     *
     * @return : The number of elements this buffer object contains or -1 if this object is null.
     */
    int size() const;


    /*!
     * Reads this object's OpenCL buffer data to host memory located in the given qt vector. The
     * command to read is sent to the given command queue and is not complete until the returned
     * event is done. This object must not be null and the given vector must be the same size as
     * this buffer.
     *
     * @param queue : Pointer to the command queue used to add the OpenCL command.
     *
     * @param vector : The vector whose data is overwritten with this OpenCL buffer once the read
     *                 command is finished.
     *
     * @return : The event for the read command sent to the given command queue.
     */
    Event read(
        CommandQueue* queue
        ,QVector<T>* vector
    );


    /*!
     * Writes to this object's OpenCL buffer data from host memory located in the given qt vector.
     * The command to write is sent to the given command queue and is not complete until the
     * returned event is done. This object must not be null and the given vector must be the same
     * size as this buffer.
     *
     * @param queue : Pointer to the command queue used to add the OpenCL command.
     *
     * @param vector : The vector whose data is written to this OpenCL buffer once the write command
     *                 is finished.
     *
     * @return : The event for the write command sent to the given command queue.
     */
    Event write(
        CommandQueue* queue
        ,const QVector<T>& vector
    );


    /*!
     * Maps this object's OpenCL buffer data to a host memory pointer for reading that can be
     * accessed through other methods of this class. The command to map memory is sent to the given
     * command queue and is not complete until the returned event is complete. If this object is
     * null or is already mapped then an exception is thrown.
     *
     * @param queue : Pointer to the command queue used to add the OpenCL command for mapping the
     *                memory.
     *
     * @return : The event for the mapping command sent to the given command queue.
     */
    Event mapRead(
        CommandQueue* queue
    );


    /*!
     * Maps this object's OpenCL buffer data to a host memory pointer for writing that can be
     * accessed through other methods of this class. The command to map memory is sent to the given
     * command queue and is not complete until the returned event is complete. If this object is
     * null or is already mapped then an exception is thrown.
     *
     * @param queue : Pointer to the command queue used to add the OpenCL command for mapping the
     *                memory.
     *
     * @return : The event for the mapping command sent to the given command queue.
     */
    Event mapWrite(
        CommandQueue* queue
    );


    /*!
     * Unmaps this object's OpenCL buffer data from host memory. The command to unmap memory is sent
     * to the given command queue and is not complete until the returned event is complete. If the
     * object is null or is not mapped then an exception is thrown.
     *
     * @param queue : Pointer to the command queue used to add the OpenCL command for unmapping the
     *                memory.
     *
     * @return : The event for the unmapping command sent to the given command queue.
     */
    Event unmap(
        CommandQueue* queue
    );


    /*!
     * Returns the host memory pointer of this object's mapped OpenCL buffer data. If this object's
     * buffer is not mapped then an exception is thrown. Care must be taken to only read or write to
     * this pointer depending on what type of mapping it is. Reading from a write mapping or writing
     * to a read mapping is undefined.
     *
     * @return : Pointer to host memory mapping of this object's OpenCL buffer.
     */
    T* data();


/***********/
/* Methods */
/***********/
private:


    /*!
     * Maps this object's OpenCL buffer to host memory using the given command queue to add the
     * command and the given mapping to set it for reading or writing. If this object is null or is
     * already mapped then an exception is thrown.
     *
     * @param queue : Pointer to the command queue used to add the mapping command.
     *
     * @param mapping : The type of mapping used, either read or write.
     *
     * @return : The event for the mapping command added to the given command queue.
     */
    Event map(
        CommandQueue* queue
        ,cl_map_flags mapping
    );


    /*!
     * Clears any mapping of this object's OpenCL buffer if it has one, releases all OpenCL
     * resources, and deletes the OpenCL memory ID variable. This does not set any of this object's
     * pointers to null.
     */
    void clear();


    /*!
     * Sets this object to a null state by changing all its pointers to null. This does not free any
     * allocated memory those pointers may be pointing to.
     */
    void nullify();


/*************/
/* Variables */
/*************/
private:


    /*!
     * Pointer to the OpenCL buffer ID of this object. If this object is null then this is null.
     */
    cl_mem* _id {nullptr};


    /*!
     * Host memory pointer to this object's mapped OpenCL buffer. If this object is not mapped then
     * this is null.
     */
    T* _data {nullptr};


    /*!
     * Size of this object's OpenCL buffer as the number of elements of the defined template type.
     */
    int _size {-1};


    /*!
     * OpenCL command queue ID of the last command queue to be used for mapping this object's
     * buffer. This is used to unmap the buffer if this object is destroyed.
     */
    cl_command_queue _last;


    /*!
     * The type of mapping of this object's OpenCL buffer if it is mapped. If this object is not
     * mapped this is not used and undefined.
     */
    cl_mem_flags _mapping;
};




template<class T>
Buffer<T>::Buffer(
    Context* context
    ,int size
)
:
    _id(new cl_mem),
    _size(size)
{
    EDEBUG_FUNC(this,context,size);
    cl_int code;
    *_id = clCreateBuffer(context->id(),CL_MEM_READ_WRITE,sizeof(T)*_size,nullptr,&code);
    if (code!=CL_SUCCESS)
    {
        E_MAKE_EXCEPTION(e);
        fillException(&e,code);
        throw e;
    }
}




template<class T>
Buffer<T>::Buffer(
    Buffer<T>&& other
)
:
    _id(other._id),
    _data(other._data),
    _size(other._size),
    _last(other._last),
    _mapping(other._mapping)
{
    EDEBUG_FUNC(this,&other);
    other.nullify();
}




template<class T>
Buffer<T>::~Buffer()
{
    EDEBUG_FUNC(this);
    clear();
}




template<class T>
void Buffer<T>::operator=(
    Buffer<T>&& other
)
{
    EDEBUG_FUNC(this,&other);
    clear();
    _id = other._id;
    _data = other._data;
    _size = other._size;
    _last = other._last;
    _mapping = other._mapping;
    other.nullify();
}




template<class T>
T& Buffer<T>::operator[](
    int index
)
{
    EDEBUG_FUNC(this,index);
    if (!_data)
    {
        E_MAKE_EXCEPTION(e);
        e.setTitle(QObject::tr("Logic Error"));
        e.setDetails(QObject::tr("Cannot access data from unmapped OpenCL buffer."));
        throw e;
    }
    if (_mapping!=CL_MAP_WRITE)
    {
        E_MAKE_EXCEPTION(e);
        e.setTitle(QObject::tr("Logic Error"));
        e.setDetails(QObject::tr("Cannot write data to OpenCL buffer mapped for reading."));
        throw e;
    }
    if (index<0 || index>=_size)
    {
        E_MAKE_EXCEPTION(e);
        e.setTitle(QObject::tr("Out Of Range"));
        e.setDetails(
            QObject::tr("The index %1 is out of range for this OpenCL buffer (%2 size).")
                .arg(index)
                .arg(_size)
        );
        throw e;
    }
    return _data[index];
}




template<class T>
const T& Buffer<T>::at(
    int index
) const
{
    EDEBUG_FUNC(this,index);
    if (!_data)
    {
        E_MAKE_EXCEPTION(e);
        e.setTitle(QObject::tr("Logic Error"));
        e.setDetails(QObject::tr("Cannot access data from unmapped OpenCL buffer."));
        throw e;
    }
    if (_mapping!=CL_MAP_READ)
    {
        E_MAKE_EXCEPTION(e);
        e.setTitle(QObject::tr("Logic Error"));
        e.setDetails(QObject::tr("Cannot read data from OpenCL buffer mapped for writing."));
        throw e;
    }
    if ( index<0 || index>=_size )
    {
        E_MAKE_EXCEPTION(e);
        e.setTitle(QObject::tr("Out Of Range"));
        e.setDetails(
            (QObject::tr("The index %1 is out of range for this OpenCL buffer (%2 size).")
                .arg(index)
                .arg(_size)
            )
        );
        throw e;
    }
    return _data[index];
}




template<class T>
bool Buffer<T>::isNull() const
{
    EDEBUG_FUNC(this);
    return !_id;
}




template<class T>
cl_mem Buffer<T>::id() const
{
    EDEBUG_FUNC(this);
    if (!_id)
    {
        E_MAKE_EXCEPTION(e);
        e.setTitle(QObject::tr("Logic Error"));
        e.setDetails(QObject::tr("Cannot return OpenCL buffer ID of null object."));
        throw e;
    }
    return *_id;
}




template<class T>
int Buffer<T>::size() const
{
    EDEBUG_FUNC(this);
    return _size;
}




template<class T>
Event Buffer<T>::read(
    CommandQueue* queue
    ,QVector<T>* vector
)
{
    EDEBUG_FUNC(this,queue,&vector);
    Q_ASSERT(vector);
    if (!_id)
    {
        E_MAKE_EXCEPTION(e);
        e.setTitle(QObject::tr("Logic Error"));
        e.setDetails(QObject::tr("Cannot read OpenCL buffer that is null."));
        throw e;
    }
    if (vector->size() != _size)
    {
        E_MAKE_EXCEPTION(e);
        e.setTitle(QObject::tr("Logic Error"));
        e.setDetails(QObject::tr("Given vector and OpenCL buffer sizes are different."));
        throw e;
    }
    cl_int code;
    cl_event id;
    code = clEnqueueReadBuffer(
        queue->id()
        ,*_id
        ,false
        ,0
        ,sizeof(T)*_size
        ,vector->data()
        ,0
        ,nullptr
        ,&id
    );
    if (code!=CL_SUCCESS)
    {
        E_MAKE_EXCEPTION(e);
        fillException(&e,code);
        throw e;
    }
    return Event(id);
}




template<class T>
Event Buffer<T>::write(
    CommandQueue* queue
    ,const QVector<T>& vector
)
{
    EDEBUG_FUNC(this,queue,&vector);
    if (!_id)
    {
        E_MAKE_EXCEPTION(e);
        e.setTitle(QObject::tr("Logic Error"));
        e.setDetails(QObject::tr("Cannot write OpenCL buffer that is null."));
        throw e;
    }
    if (vector.size() != _size)
    {
        E_MAKE_EXCEPTION(e);
        e.setTitle(QObject::tr("Logic Error"));
        e.setDetails(QObject::tr("Given vector and OpenCL buffer sizes are different."));
        throw e;
    }
    cl_int code;
    cl_event id;
    code = clEnqueueWriteBuffer(
        queue->id()
        ,*_id
        ,false
        ,0
        ,sizeof(T)*_size
        ,vector.data()
        ,0
        ,nullptr
        ,&id
    );
    if (code!=CL_SUCCESS)
    {
        E_MAKE_EXCEPTION(e);
        fillException(&e,code);
        throw e;
    }
    return Event(id);
}




template<class T>
Event Buffer<T>::mapRead(
    CommandQueue* queue
)
{
    EDEBUG_FUNC(this,queue);
    return map(queue,CL_MAP_READ);
}




template<class T>
Event Buffer<T>::mapWrite(
    CommandQueue* queue
)
{
    EDEBUG_FUNC(this,queue);
    return map(queue,CL_MAP_WRITE);
}




template<class T>
Event Buffer<T>::unmap(
    CommandQueue* queue
)
{
    EDEBUG_FUNC(this,queue);
    if (!_id)
    {
        E_MAKE_EXCEPTION(e);
        e.setTitle(QObject::tr("Logic Error"));
        e.setDetails(QObject::tr("Cannot unmap OpenCL buffer that is null."));
        throw e;
    }
    if (!_data)
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
    if (code!=CL_SUCCESS)
    {
        E_MAKE_EXCEPTION(e);
        fillException(&e,code);
        throw e;
    }
    return Event(id);
}




template<class T>
T* Buffer<T>::data()
{
    EDEBUG_FUNC(this);
    if (!_data)
    {
        E_MAKE_EXCEPTION(e);
        e.setTitle(QObject::tr("Logic Error"));
        e.setDetails(QObject::tr("Cannot access data from unmapped OpenCL buffer."));
        throw e;
    }
    return _data;
}




template<class T>
Event Buffer<T>::map(
    CommandQueue* queue
    ,cl_map_flags mapping
)
{
    EDEBUG_FUNC(this,queue,static_cast<int>(mapping));
    if (!_id)
    {
        E_MAKE_EXCEPTION(e);
        e.setTitle(QObject::tr("Logic Error"));
        e.setDetails(QObject::tr("Cannot unmap OpenCL buffer that is null."));
        throw e;
    }
    if (_data)
    {
        E_MAKE_EXCEPTION(e);
        e.setTitle(QObject::tr("Logic Error"));
        e.setDetails(QObject::tr("Cannot map OpenCL buffer that is already mapped."));
        throw e;
    }
    cl_int code;
    cl_event id;
    _data = (
        static_cast<T*>(
            clEnqueueMapBuffer(
                queue->id()
                ,*_id
                ,false
                ,mapping
                ,0
                ,sizeof(T)*_size
                ,0
                ,nullptr
                ,&id
                ,&code
            )
        )
    );
    if (code!=CL_SUCCESS)
    {
        E_MAKE_EXCEPTION(e);
        fillException(&e,code);
        throw e;
    }
    _last = queue->id();
    code = clRetainCommandQueue(_last);
    if (code!=CL_SUCCESS)
    {
        E_MAKE_EXCEPTION(e);
        fillException(&e,code);
        throw e;
    }
    _mapping = mapping;
    return Event(id);
}




template<class T>
void Buffer<T>::clear()
{
    EDEBUG_FUNC(this);
    if (_id)
    {
        if (_data)
        {
            clEnqueueUnmapMemObject(_last,*_id,_data,0,nullptr,nullptr);
            clReleaseCommandQueue(_last);
        }
        clReleaseMemObject(*_id);
        delete _id;
    }
}




template<class T>
void Buffer<T>::nullify()
{
    EDEBUG_FUNC(this);
    _id = nullptr;
    _data = nullptr;
    _size = -1;
    _last = nullptr;
}

}

#endif
