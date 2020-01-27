#include "opencl_platform.h"
#include "opencl_device.h"
#include "opencl.h"
#include "eexception.h"
#include "edebug.h"

namespace OpenCL
{




QList<Platform*>* Platform::_platforms {nullptr};




int Platform::size()
{
    EDEBUG_FUNC();
    populate();
    return _platforms->size();
}




Platform* Platform::get(
    int index
)
{
    EDEBUG_FUNC(index);
    populate();
    if ( index<0 || index>=_platforms->size() )
    {
        E_MAKE_EXCEPTION(e);
        e.setTitle(tr("Out Of Range"));
        e.setDetails(
            (tr("OpenCL platform index %1 is out of range (%2 platforms exist).")
                .arg(index)
                .arg(_platforms->size())
            )
        );
        throw e;
    }
    return _platforms->at(index);
}




Device* Platform::device(
    int index
) const
{
    EDEBUG_FUNC(this,index);
    if ( index<0 || index>=_devices.size() )
    {
        E_MAKE_EXCEPTION(e);
        e.setTitle(tr("Out Of Range"));
        e.setDetails(
            (tr("OpenCL device index %1 is out of range (%2 devices exist).")
                .arg(index)
                .arg(_devices.size())
            )
        );
        throw e;
    }
    return _devices.at(index);
}




int Platform::deviceSize() const
{
    EDEBUG_FUNC(this);
    return _devices.size();
}




QStringList Platform::extensions() const
{
    EDEBUG_FUNC(this);
    return _extensions;
}




cl_platform_id Platform::id() const
{
    EDEBUG_FUNC(this);
    return _id;
}




QString Platform::name() const
{
    EDEBUG_FUNC(this);
    return _name;
}




QString Platform::profile() const
{
    EDEBUG_FUNC(this);
    return _profile;
}




QString Platform::vendor() const
{
    EDEBUG_FUNC(this);
    return _vendor;
}




QString Platform::version() const
{
    EDEBUG_FUNC(this);
    return _version;
}




void Platform::populate()
{
    EDEBUG_FUNC();
    if ( !_platforms )
    {
        _platforms = new QList<Platform*>();
        cl_uint rawSize;
        cl_int code {clGetPlatformIDs(0,nullptr,&rawSize)};
        if (code!=CL_SUCCESS)
        {
            return;
        }
        Q_ASSERT(rawSize < std::numeric_limits<int>::max());
        int size {static_cast<int>(rawSize)};
        QVector<cl_platform_id> platforms(size);
        code = clGetPlatformIDs(rawSize,platforms.data(),nullptr);
        if (code!=CL_SUCCESS)
        {
            return;
        }
        for (int i = 0; i<size; ++i)
        {
            *_platforms << new Platform(platforms.at(i));
        }
    }
}




Platform::Platform(
    cl_platform_id id
    ,QObject* parent
)
:
    QObject(parent)
    ,_extensions(getInfo(CL_PLATFORM_EXTENSIONS).split(' '))
    ,_id(id)
    ,_name(getInfo(CL_PLATFORM_NAME))
    ,_profile(getInfo(CL_PLATFORM_PROFILE))
    ,_vendor(getInfo(CL_PLATFORM_VENDOR))
    ,_version(getInfo(CL_PLATFORM_VERSION))
{
    EDEBUG_FUNC(this,id,parent);
    populateDevices();
}




QString Platform::getInfo(
    cl_platform_info type
) const
{
    EDEBUG_FUNC(this,type);
    size_t rawSize;
    cl_int code {clGetPlatformInfo(_id,type,0,nullptr,&rawSize)};
    if (code!=CL_SUCCESS)
    {
        E_MAKE_EXCEPTION(e);
        fillException(&e,code);
        throw e;
    }
    Q_ASSERT(rawSize < std::numeric_limits<int>::max());
    int size {static_cast<int>(rawSize)};
    QByteArray info(size,'\0');
    code = clGetPlatformInfo(_id,type,rawSize,info.data(),nullptr);
    if (code!=CL_SUCCESS)
    {
        E_MAKE_EXCEPTION(e);
        fillException(&e,code);
        throw e;
    }
    return QString(info);
}




void Platform::populateDevices()
{
    EDEBUG_FUNC(this);
    cl_uint rawSize;
    cl_int code {clGetDeviceIDs(_id,CL_DEVICE_TYPE_ALL,0,nullptr,&rawSize)};
    if (code!=CL_SUCCESS)
    {
        E_MAKE_EXCEPTION(e);
        fillException(&e,code);
        throw e;
    }
    Q_ASSERT(rawSize < std::numeric_limits<int>::max());
    int size {static_cast<int>(rawSize)};
    QVector<cl_device_id> devices(size);
    code = clGetDeviceIDs(_id,CL_DEVICE_TYPE_ALL,rawSize,devices.data(),nullptr);
    if (code!=CL_SUCCESS)
    {
        E_MAKE_EXCEPTION(e);
        fillException(&e,code);
        throw e;
    }
    for (int i = 0; i<size; ++i)
    {
        _devices << new Device(devices.at(i),this);
    }
}

}
