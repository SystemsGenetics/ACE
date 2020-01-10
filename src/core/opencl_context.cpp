#include "opencl_context.h"
#include "opencl_platform.h"
#include "opencl_device.h"
#include "opencl.h"
#include "eexception.h"
#include "edebug.h"

namespace OpenCL
{




Context::Context(
    const QList<Device*>& devices
    ,QObject* parent
)
:
    QObject(parent),
    _devices(devices)
{
    EDEBUG_FUNC(this,&devices,parent);
    if (devices.isEmpty())
    {
        E_MAKE_EXCEPTION(e);
        e.setTitle(tr("Invalid Argument"));
        e.setDetails(tr("Cannot create OpenCL context with empty list of devices."));
        throw e;
    }
    cl_int code;
    cl_context_properties properties[] = {
        CL_CONTEXT_PLATFORM
        ,reinterpret_cast<cl_context_properties>(devices.first()->platform())
        ,0
    };
    QVector<cl_device_id> deviceIds(devices.size());
    for (int i = 0; i < devices.size() ;++i)
    {
        deviceIds[i] = devices.at(i)->id();
    }
    _id = clCreateContext(
        properties
        ,static_cast<cl_uint>(devices.size())
        ,deviceIds.data()
        ,nullptr
        ,nullptr
        ,&code
    );
    if (code!= CL_SUCCESS)
    {
        E_MAKE_EXCEPTION(e);
        fillException(&e,code);
        throw e;
    }
}




Context::~Context()
{
    EDEBUG_FUNC(this);
    clReleaseContext(_id);
}




cl_context Context::id() const
{
    EDEBUG_FUNC(this);
    return _id;
}




const QList<Device*>& Context::devices() const
{
    EDEBUG_FUNC(this);
    return _devices;
}

}
