#include "opencl_kernel.h"
#include "opencl_device.h"
#include "opencl_program.h"
#include "opencl_commandqueue.h"
#include "opencl_event.h"
#include "opencl.h"
#include "ace_settings.h"
#include "eexception.h"

namespace OpenCL
{




Kernel::Kernel(
    Program* program
    ,const QString& name
    ,QObject* parent
)
:
    QObject(parent)
{
    EDEBUG_FUNC(this,program,name,parent);
    cl_int code;
    _id = clCreateKernel(program->id(),name.toLocal8Bit().data(),&code);
    if (code!=CL_SUCCESS)
    {
        E_MAKE_EXCEPTION(e);
        fillException(&e,code);
        throw e;
    }
    allocate();
}




Kernel::~Kernel()
{
    EDEBUG_FUNC(this);
    clReleaseKernel(_id);
    clear();
}




Event Kernel::execute(
    CommandQueue* queue
)
{
    EDEBUG_FUNC(this,queue);
    cl_event id;
    cl_int code
    {
        clEnqueueNDRangeKernel(
            queue->id()
            ,_id
            ,_size
            ,_offsets
            ,_globalSizes
            ,_localSizes
            ,0
            ,nullptr
            ,&id
        )
    };
    if (code!=CL_SUCCESS)
    {
        E_MAKE_EXCEPTION(e);
        fillException(&e,code);
        throw e;
    }
    return Event(id);
}




Kernel::Locker Kernel::lock()
{
    EDEBUG_FUNC(this);
    _lock.lock();
    _isLocked = true;
    return Locker(this);
}




int Kernel::maxWorkGroupSize(
    Device* device
) const
{
    EDEBUG_FUNC(this,device);
    if ( !_isLocked )
    {
        E_MAKE_EXCEPTION(e);
        e.setTitle(tr("Logic Error"));
        e.setDetails(tr("Cannot set OpenCL kernel parameters without locking the object first."));
        throw e;
    }
    size_t size;
    cl_int code
    {
        clGetKernelWorkGroupInfo(
            _id
            ,device->id()
            ,CL_KERNEL_WORK_GROUP_SIZE
            ,sizeof(size_t)
            ,&size
            ,nullptr
        )
    };
    if (code!=CL_SUCCESS)
    {
        E_MAKE_EXCEPTION(e);
        fillException(&e,code);
        throw e;
    }
    Q_ASSERT(size < std::numeric_limits<int>::max());
    return static_cast<int>(size);
}




void Kernel::setDimensions(
    cl_uint size
)
{
    EDEBUG_FUNC(this,size);
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
            (
                tr("%1 is not a valid dimension size for an OpenCL Kernel (must be 1 or geater).")
                .arg(size)
            )
        );
        throw e;
    }
    if (size!=_size)
    {
    _size = size;
    clear();
    allocate();
    }
}




void Kernel::setSizes(
    cl_uint dimension
    ,qint64 globalSize
    ,qint64 localSize
)
{
    EDEBUG_FUNC(this,dimension,globalSize,localSize);
    if (!_isLocked)
    {
        E_MAKE_EXCEPTION(e);
        e.setTitle(tr("Logic Error"));
        e.setDetails(tr("Cannot set OpenCL kernel parameters without locking the object first."));
        throw e;
    }
    if (dimension>=_size)
    {
        E_MAKE_EXCEPTION(e);
        e.setTitle(tr("Out Of Range"));
        e.setDetails(
            (
                tr("Cannot set dimension %1 sizes of OpenCL kernel (%2 total).")
                .arg(dimension)
                .arg(_size)
            )
        );
        throw e;
    }
    if ( globalSize<1 || localSize<1 || globalSize%localSize )
    {
        E_MAKE_EXCEPTION(e);
        e.setTitle(tr("Invalid Argument"));
        e.setDetails(
            (
                tr("%1 and %2 are not valid global and local OpenCL kernel sizes, respectively.")
                .arg(globalSize)
                .arg(localSize)
            )
        );
        throw e;
    }
    _globalSizes[dimension] = static_cast<size_t>(globalSize);
    _localSizes[dimension] = static_cast<size_t>(localSize);
}




void Kernel::unlock()
{
    EDEBUG_FUNC(this);
    _lock.unlock();
    _isLocked = false;
}




int Kernel::workGroupMultiple(
    Device* device
) const
{
    EDEBUG_FUNC(this,device);
    if (!_isLocked)
    {
        E_MAKE_EXCEPTION(e);
        e.setTitle(tr("Logic Error"));
        e.setDetails(tr("Cannot set OpenCL kernel parameters without locking the object first."));
        throw e;
    }
    size_t size;
    cl_int code
    {
        clGetKernelWorkGroupInfo(
            _id
            ,device->id()
            ,CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE
            ,sizeof(size_t)
            ,&size
            ,nullptr
        )
    };
    if (code!=CL_SUCCESS)
    {
        E_MAKE_EXCEPTION(e);
        fillException(&e,code);
        throw e;
    }
    Q_ASSERT(size < std::numeric_limits<int>::max());
    return static_cast<int>(size);
}




void Kernel::allocate()
{
    EDEBUG_FUNC(this);
    _offsets = new size_t[_size];
    _globalSizes = new size_t[_size];
    _localSizes = new size_t[_size];
    for (cl_uint i = 0; i<_size; ++i)
    {
        _offsets[i] = 0;
        _globalSizes[i] = 1;
        _localSizes[i] = 1;
    }
}




void Kernel::clear()
{
    EDEBUG_FUNC(this);
    delete[] _offsets;
    delete[] _globalSizes;
    delete[] _localSizes;
}




Kernel::Locker::Locker(
    Kernel* kernel
)
:
    _kernel(kernel)
{
    EDEBUG_FUNC(this,kernel);
}




Kernel::Locker::Locker(
    Locker&& other
)
:
    _kernel(other._kernel)
{
    EDEBUG_FUNC(this,&other);
    other._kernel = nullptr;
}




Kernel::Locker::~Locker()
{
    EDEBUG_FUNC(this);
    if (_kernel)
    {
        _kernel->unlock();
    }
}

}
