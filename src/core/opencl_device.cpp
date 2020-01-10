#include "opencl_device.h"
#include "opencl.h"

namespace OpenCL
{




Device::Device(
    cl_device_id id
    ,QObject* parent
)
:
    QObject(parent),
    _id(id),
    _available(getInfo<bool,cl_bool>(CL_DEVICE_AVAILABLE)),
    _compiler(getInfo<bool,cl_bool>(CL_DEVICE_COMPILER_AVAILABLE)),
    _extensions(getStringInfo(CL_DEVICE_EXTENSIONS).split(' ')),
    _globalMemorySize(getInfo<qint64,cl_ulong>(CL_DEVICE_GLOBAL_MEM_SIZE)),
    _localMemorySize(getInfo<qint64,cl_ulong>(CL_DEVICE_LOCAL_MEM_SIZE)),
    _computeUnitSize(getInfo<int,cl_uint>(CL_DEVICE_MAX_COMPUTE_UNITS)),
    _clockFrequency(getInfo<int,cl_uint>(CL_DEVICE_MAX_CLOCK_FREQUENCY)),
    _name(getStringInfo(CL_DEVICE_NAME)),
    _platform(getInfo<cl_platform_id,cl_platform_id>(CL_DEVICE_PLATFORM))
{
    EDEBUG_FUNC(this,static_cast<void*>(id),parent);
    switch (getInfo<cl_device_type,cl_device_type>(CL_DEVICE_TYPE))
    {
    case CL_DEVICE_TYPE_CPU:
        _type = Type::CPU;
        break;
    case CL_DEVICE_TYPE_GPU:
        _type = Type::GPU;
        break;
    case CL_DEVICE_TYPE_ACCELERATOR:
        _type = Type::Accelerator;
        break;
    case CL_DEVICE_TYPE_DEFAULT:
        _type = Type::Default;
        break;
    }
}




cl_device_id Device::id() const
{
    EDEBUG_FUNC(this);
    return _id;
}




bool Device::isAvailable() const
{
    EDEBUG_FUNC(this);
    return _available;
}




bool Device::hasCompiler() const
{
    EDEBUG_FUNC(this);
    return _compiler;
}




const QStringList& Device::extensions() const
{
    EDEBUG_FUNC(this);
    return _extensions;
}




qint64 Device::globalMemorySize() const
{
    EDEBUG_FUNC(this);
    return _globalMemorySize;
}




qint64 Device::localMemorySize() const
{
    EDEBUG_FUNC(this);
    return _localMemorySize;
}




int Device::computeUnitSize() const
{
    EDEBUG_FUNC(this);
    return _computeUnitSize;
}




int Device::clockFrequency() const
{
    EDEBUG_FUNC(this);
    return _clockFrequency;
}




QString Device::name() const
{
    EDEBUG_FUNC(this);
    return _name;
}




Device::Type Device::type() const
{
    EDEBUG_FUNC(this);
    return _type;
}




cl_platform_id Device::platform() const
{
    EDEBUG_FUNC(this);
    return _platform;
}




QString Device::getStringInfo(
    cl_device_info type
) const
{
    EDEBUG_FUNC(this,type);
    size_t size;
    cl_int code {clGetDeviceInfo(_id,type,0,nullptr,&size)};
    if (code!=CL_SUCCESS)
    {
        E_MAKE_EXCEPTION(e);
        fillException(&e,code);
        throw e;
    }
    Q_ASSERT(size<static_cast<size_t>(std::numeric_limits<int>::max()));
    QByteArray data(static_cast<int>(size),'\0');
    code = clGetDeviceInfo(_id,type,size,data.data(),nullptr);
    if (code!=CL_SUCCESS)
    {
        E_MAKE_EXCEPTION(e);
        fillException(&e,code);
        throw e;
    }
    return QString(data);
}

}
