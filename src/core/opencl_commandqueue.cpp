#include "opencl_commandqueue.h"
#include "opencl_context.h"
#include "opencl_device.h"
#include "opencl_event.h"
#include "opencl.h"
#include "eexception.h"
#include "edebug.h"

namespace OpenCL
{




CommandQueue::CommandQueue(
    Context* context
    ,Device* device
    ,QObject* parent
)
    :
    QObject(parent),
    _device(device)
{
    EDEBUG_FUNC(this,context,device,parent);
    cl_int code;
    _id = (
        clCreateCommandQueue(
            context->id()
            ,device->id()
            ,CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE
            ,&code
        )
    );
    if (code!=CL_SUCCESS)
    {
        E_MAKE_EXCEPTION(e);
        fillException(&e,code);
        throw e;
    }
}




CommandQueue::~CommandQueue()
{
   EDEBUG_FUNC(this);
   clReleaseCommandQueue(_id);
}




cl_command_queue CommandQueue::id() const
{
   EDEBUG_FUNC(this);
   return _id;
}




Device* CommandQueue::device() const
{
   EDEBUG_FUNC(this);
   return _device;
}




void CommandQueue::wait()
{
   EDEBUG_FUNC(this);
   cl_int code;
   cl_event id;
   code = clEnqueueMarkerWithWaitList(_id,0,nullptr,&id);
   if (code!=CL_SUCCESS)
   {
      E_MAKE_EXCEPTION(e);
      fillException(&e,code);
      throw e;
   }
   Event event(id);
   event.wait();
}

}
