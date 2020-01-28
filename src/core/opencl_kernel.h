#ifndef OPENCL_KERNEL_H
#define OPENCL_KERNEL_H
#include <CL/cl.h>
#include <QObject>
#include "opencl_buffer.h"
#include "opencl.h"
#include "edebug.h"

namespace OpenCL
{




/*!
 * This contains an OpenCL kernel. This class is designed to be inherited and implemented by
 * specific kernel implementations. This design works to facilitate separating the more arduous task
 * of setting of a kernel to execute. This way arguments for a kernel can be abstracted to C++ with
 * easy to understand arguments for methods. Because of this design most methods to set the kernel
 * arguments and work sizes is protected and should only be used by a child inheriting this class.
 * Setting kernel parameters is also the only part of OpenCL which is not thread safe. Because of
 * this setting parameters is protected behind a mutex and parameters can only be set by a thread
 * when it has locked that mutex.
 */
class Kernel
:
    public QObject
{
    Q_OBJECT


/****************/
/* Constructors */
/****************/
public:


    /*!
     * Constructs a new kernel object from the given program with the given kernel name and optional
     * parent.
     *
     * @param program : Pointer to the program which has built the kernel with the given name that
     *                  is created.
     *
     * @param name : The name of the kernel that is created.
     *
     * @param parent : Optional parent for this new kernel object.
     */
    explicit Kernel(
        Program* program
        ,const QString& name
        ,QObject* parent=nullptr
    );


/**************/
/* Destructor */
/**************/
public:


    /*!
     * Clears all resources this kernel contains.
     */
    virtual ~Kernel() override;


/***********/
/* Methods */
/***********/
public:


    /*!
     * Executes this object's OpenCL kernel on the given command queue with the dimensions, global,
     * and local sizes this object possesses, returning the event for the kernel command.
     *
     * @param queue : The OpenCL command queue used to execute this kernel.
     *
     * @return : The event for the kernel command running on the given command queue.
     */
    Event execute(
        CommandQueue* queue
    );


/***********/
/* Methods */
/***********/
protected:
    class Locker;


    /*!
     * Locks this kernel object allowing the setting of this object's kernel parameters and
     * returning a locker object that unlocks this kernel on its destruction.
     *
     * @return : Locker object that unlocks this kernel upon its destruction.
     */
    Kernel::Locker lock();


    /*!
     * Returns the maximum work group (local) size this kernel can possess for the given device. If
     * this kernel is not locked then an exception is thrown.
     *
     * @param device : The device whose maximum work group size is returned.
     *
     * @return : Maximum work group size this kernel can possess for the given device.
     */
    int maxWorkGroupSize(
        Device* device
    ) const;


    /*!
     * Sets this kernel's argument at the given index to the given value. The first argument of a
     * kernel function is at index 0 and increments positively. If this kernel is not locked then an
     * exception is thrown.
     *
     * @tparam T : The type of the value that is set.
     *
     * @param index : The kernel argument index whose value is set.
     *
     * @param value : The value that the given kernel argument is set to.
     */
    template<class T>
    void setArgument(
        cl_uint index
        ,T value
    );


    /*!
     * Sets this kernel's argument at the given index to the given OpenCL buffer. The first argument
     * of a kernel function is at index 0 and increments positively. If this kernel is not locked
     * then an exception is thrown.
     *
     * @tparam T : The buffer type that is set.
     *
     * @param index : The kernel argument index whose value is set.
     *
     * @param buffer : The buffer object that the given kernel argument is set to.
     */
    template<class T>
    void setBuffer(
        cl_uint index
        ,Buffer<T>* buffer
    );


    /*!
     * Sets the number of dimensions for parallel execution of this kernel object. If this kernel is
     * not locked or the given size is less than one then an exception is thrown.
     *
     * @param size : The number of dimensions for this kernel.
     */
    void setDimensions(
        cl_uint size
    );


    /*!
     * Sets this kernel's argument at the given index to local memory allocated to the given size in
     * elements of template type. The first argument of a kernel function is at index 0 and
     * increments positively. If this kernel is not locked or the given size is less than one then
     * an exception is thrown.
     *
     * @tparam T : The element type whose given size in elements is allocated.
     *
     * @param index : The kernel argument index whose value is set.
     *
     * @param size : The number of elements of template type which is allocated in local memory.
     */
    template<class T>
    void setLocalMemory(
        cl_uint index
        ,qint64 size
    );


    /*!
     * Sets the global and local sizes of the given dimension used for parallel execution of this
     * kernel object. If this kernel is not locked, the dimension is invalid, the local or group
     * size is less than one, or the global size is not a multiple of the local size then an
     * exception is thrown.
     *
     * @param dimension : The dimension whose global and local sizes are set.
     *
     * @param globalSize : The new global size that is set. This must be a multiple of the local
     *                     size.
     *
     * @param localSize : The new local or work group size that is set. This must be divisible of
     *                    the global size.
     */
    void setSizes(
        cl_uint dimension
        ,qint64 globalSize
        ,qint64 localSize
    );


    /*!
     * Unlocks this kernel object after which parameters cannot be set and allows other threads to
     * lock it. This must never be called directly, instead the Locker class calls this upon its
     * destruction.
     */
    void unlock();


    /*!
     * Returns the recommended, for efficiency, work group multiple for the given device. If this
     * kernel is not locked then an exception is thrown.
     *
     * @param device : The device whose recommended work group multiple is returned.
     *
     * @return : Work group multiple for the given device.
     */
    int workGroupMultiple(
        Device* device
    ) const;


/***********/
/* Methods */
/***********/
private:


    /*!
     * Allocates new arrays for the offsets, global sizes, and local sizes of this object used for
     * adding a kernel parallel execution command. The dimension size is used for the new sizes. Any
     * memory pointed to previously is overwritten and not deleted.
     */
    void allocate();


    /*!
     * Deletes arrays pointed to by this object's offsets, global sizes, and local sizes array
     * pointers.
     */
    void clear();


/*************/
/* Variables */
/*************/
private:


    /*!
     * The global size for each dimension used by this kernel object for execution. The default is
     * 1.
     */
    size_t* _globalSizes {nullptr};


    /*!
     * The OpenCL kernel ID of this object.
     */
    cl_kernel _id;


    /*!
     * True if this kernel's mutex is locked for configuring this kernels parameters or false
     * otherwise.
     */
    bool _isLocked {false};


    /*!
     * The local or work group size for each dimension used by this kernel object for execution. The
     * default is 1.
     */
    size_t* _localSizes {nullptr};


    /*!
     * This protects this object's setting of parameters which is not thread safe in OpenCL.
     * Parameters can only be set when this is locked.
     */
    QMutex _lock;


    /*!
     * The offsets for each dimension used by this kernel object for execution. The offsets are
     * always 0.
     */
    size_t* _offsets {nullptr};


    /*!
     * The number of dimensions this kernel object uses when executes.
     */
    cl_uint _size {1};


/***********/
/* Classes */
/***********/
protected:




    /*!
     * This holds a lock on the given kernel object making it easy to manage the locked resource and
     * avoid very hard to avoid deadlock bugs in complicated or exception handling code. This class
     * can move its ownership of a kernel lock to another instance but it cannot be copied. Upon
     * destruction this class calls the unlock method of the kernel object it contains.
     */
    class Locker
    {


    /****************/
    /* Constructors */
    /****************/
    public:


        /*!
         * Constructs a new locker object with the given locked kernel object.
         *
         * @param kernel : The kernel object that must be locked.
         */
        Locker(
            Kernel* kernel
        );


        /*!
         * Constructs a new locker object taking ownership of the other locker object's kernel lock.
         *
         * @param other : The other locker instance whose locked kernel object is moved to this one.
         */
        Locker(
            Locker&& other
        );


    /**************/
    /* Destructor */
    /**************/
    public:


        /*!
         * Unlocks the kernel object this locker object contains.
         */
        ~Locker();


    /*************/
    /* Variables */
    /*************/
    private:


        /*!
         * Pointer to the kernel object that is assumed to be locked and this object will unlock
         * upon destruction.
         */
        Kernel* _kernel;
    };
};




template<class T>
void Kernel::setArgument(
    cl_uint index
    ,T value
)
{
    EDEBUG_FUNC(this,index,value);
    if (!_isLocked)
    {
        E_MAKE_EXCEPTION(e);
        e.setTitle(tr("Logic Error"));
        e.setDetails(tr("Cannot set OpenCL kernel parameters without locking the object first."));
        throw e;
    }
    cl_int code {clSetKernelArg(_id,index,sizeof(T),&value)};
    if (code!=CL_SUCCESS)
    {
        E_MAKE_EXCEPTION(e);
        fillException(&e,code);
        throw e;
    }
}




template<class T>
void Kernel::setBuffer(
    cl_uint index
    ,Buffer<T>* buffer
)
{
    EDEBUG_FUNC(this,index,buffer);
    if (!_isLocked)
    {
        E_MAKE_EXCEPTION(e);
        e.setTitle(tr("Logic Error"));
        e.setDetails(tr("Cannot set OpenCL kernel parameters without locking the object first."));
        throw e;
    }
    cl_mem id {buffer->id()};
    cl_int code {clSetKernelArg(_id,index,sizeof(cl_mem),&id)};
    if (code!=CL_SUCCESS)
    {
        E_MAKE_EXCEPTION(e);
        fillException(&e,code);
        throw e;
    }
}




template<class T>
void Kernel::setLocalMemory(
    cl_uint index
    ,qint64 size
)
{
    EDEBUG_FUNC(this,index,size);
    if (!_isLocked)
    {
        E_MAKE_EXCEPTION(e);
        e.setTitle(tr("Logic Error"));
        e.setDetails(tr("Cannot set OpenCL kernel parameters without locking the object first."));
        throw e;
    }
    if (size<1)
    {
        E_MAKE_EXCEPTION(e);
        e.setTitle(tr("Invalid Argument"));
        e.setDetails(
            tr("Cannot set local memory argument of OpenCL kernel with a size of less than 1.")
        );
        throw e;
    }
    cl_int code {clSetKernelArg(_id,index,sizeof(T)*size,nullptr)};
    if (code!=CL_SUCCESS)
    {
        E_MAKE_EXCEPTION(e);
        fillException(&e,code);
        throw e;
    }
}

}

#endif
