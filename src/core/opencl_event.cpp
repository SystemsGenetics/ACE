#include "opencl_event.h"
#include "opencl.h"
#include "eexception.h"
#include "edebug.h"

namespace OpenCL
{




Event::Event(
    cl_event id
)
:
    _id(new cl_event)
{
    EDEBUG_FUNC(this,static_cast<void*>(id));
    *_id = id;
}




Event::Event(
    const Event& other
)
{
    EDEBUG_FUNC(this,&other);
    if (other._id)
    {
        *_id = *other._id;
        retain();
    }
}




Event::Event(
    Event&& other
)
:
    _id(other._id)
{
    EDEBUG_FUNC(this,&other);
    other._id = nullptr;
}




Event::~Event()
{
    EDEBUG_FUNC(this);
    if (_id)
    {
        clReleaseEvent(*_id);
        delete _id;
    }
}




void Event::operator=(
    const Event& other
)
{
    EDEBUG_FUNC(this,&other);
    if (_id)
    {
        release();
    }
    if (other._id)
    {
        if (!_id)
        {
            _id = new cl_event;
        }
        *_id = *other._id;
        retain();
    }
    else
    {
        delete _id;
        _id = nullptr;
    }
}




void Event::operator=(
    Event&& other
)
{
    EDEBUG_FUNC(this,&other);
    if (_id)
    {
        release();
        delete _id;
    }
    _id = other._id;
    other._id = nullptr;
}




bool Event::isNull() const
{
   EDEBUG_FUNC(this);
   return !_id;
}




void Event::wait() const
{
    EDEBUG_FUNC(this);
    if (_id)
    {
        cl_int code {clWaitForEvents(1,_id)};
        if (code!=CL_SUCCESS)
        {
            E_MAKE_EXCEPTION(e);
            fillException(&e,code);
            throw e;
        }
    }
}




bool Event::isDone() const
{
    EDEBUG_FUNC(this);
    if (!_id)
    {
        return true;
    }
    cl_int status;
    cl_int code {
        clGetEventInfo(*_id,CL_EVENT_COMMAND_EXECUTION_STATUS,sizeof(cl_int),&status,nullptr)
    };
    if (code!=CL_SUCCESS)
    {
        E_MAKE_EXCEPTION(e);
        fillException(&e,code);
        throw e;
    }
    return status == CL_COMPLETE;
}




void Event::release()
{
    EDEBUG_FUNC(this);
    cl_int code {clRetainEvent(*_id)};
    if (code!=CL_SUCCESS)
    {
        E_MAKE_EXCEPTION(e);
        fillException(&e,code);
        throw e;
    }
}




void Event::retain()
{
    EDEBUG_FUNC(this);
    cl_int code {clRetainEvent(*_id)};
    if (code!=CL_SUCCESS)
    {
        E_MAKE_EXCEPTION(e);
        fillException(&e,code);
        throw e;
    }
}

}
