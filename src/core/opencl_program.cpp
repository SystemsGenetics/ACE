#include "opencl_program.h"
#include "opencl_context.h"
#include "opencl_device.h"
#include "opencl.h"
#include "eexception.h"
#include "edebug.h"

namespace OpenCL
{




Program::Program(
    Context* context
    ,const QStringList& paths
    ,QObject* parent
)
:
    QObject(parent)
{
    EDEBUG_FUNC(this,context,paths,parent);
    QVector<const char*> sources(paths.size(),nullptr);
    try
    {
        QVector<size_t> sizes(paths.size());
        for (int i = 0; i<paths.size(); ++i)
        {
            sources[i] = readSourceFile(paths.at(i),&sizes[i]);
        }
        cl_int code;
        _id = clCreateProgramWithSource(
            context->id()
            ,static_cast<cl_uint>(paths.size())
            ,sources.data()
            ,sizes.data()
            ,&code
        );
        if ( code != CL_SUCCESS )
        {
            E_MAKE_EXCEPTION(e);
            fillException(&e,code);
            throw e;
        }
        for (auto device: context->devices())
        {
            build(device);
        }
        for (auto ptr: sources)
        {
            delete[] ptr;
        }
    }
    catch (...)
    {
        for (auto ptr: sources)
        {
            delete[] ptr;
        }
        throw;
    }
}




Program::~Program()
{
    EDEBUG_FUNC(this);
    clReleaseProgram(_id);
}




cl_program Program::id() const
{
    EDEBUG_FUNC(this);
    return _id;
}




void Program::build(
    Device* device
)
{
    EDEBUG_FUNC(this,device)
    cl_device_id id {device->id()};
    cl_int code {clBuildProgram(_id,1,&id,"",nullptr,nullptr)};
    if (code==CL_BUILD_PROGRAM_FAILURE)
    {
        E_MAKE_EXCEPTION(e);
        e.setTitle(tr("OpenCL Build Failure"));
        e.setDetails(buildLog(device));
        throw e;
    }
    else if (code!=CL_SUCCESS)
    {
        E_MAKE_EXCEPTION(e);
        fillException(&e,code);
        throw e;
    }
}




QString Program::buildLog(
    Device* device
) const
{
    EDEBUG_FUNC(this,device)
    size_t size;
    cl_int code {clGetProgramBuildInfo(_id,device->id(),CL_PROGRAM_BUILD_LOG,0,nullptr,&size)};
    if (code!=CL_SUCCESS)
    {
        E_MAKE_EXCEPTION(e);
        fillException(&e,code);
        throw e;
    }
    Q_ASSERT(size < std::numeric_limits<int>::max());
    QByteArray log(static_cast<int>(size),'\0');
    code = clGetProgramBuildInfo(_id,device->id(),CL_PROGRAM_BUILD_LOG,size,log.data(),nullptr);
    if (code!=CL_SUCCESS)
    {
        E_MAKE_EXCEPTION(e);
        fillException(&e,code);
        throw e;
    }
    return log;
}




char* Program::readSourceFile(
    const QString& path
    ,size_t* size
)
{
    EDEBUG_FUNC(this,path,size)
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        E_MAKE_EXCEPTION(e);
        e.setTitle(tr("System Error"));
        e.setDetails(
            (tr("Failed opening OpenCL kernel source code file %1: %2")
                .arg(path)
                .arg(file.errorString())
            )
        );
        throw e;
    }
    QByteArray source {file.readAll()};
    char* string {new char[source.size()]};
    memcpy(string,source.data(),static_cast<size_t>(source.size()));
    *size = static_cast<size_t>(source.size());
    return string;
}

}
