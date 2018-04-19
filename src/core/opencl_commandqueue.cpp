#include "opencl_commandqueue.h"
#include "opencl_context.h"
#include "opencl_device.h"
#include "opencl_common.h"
#include "eexception.h"



using namespace OpenCL;
//






/*!
 *
 * @param context  
 *
 * @param device  
 *
 * @param parent  
 */
CommandQueue::CommandQueue(Context* context, Device* device, QObject* parent):
   QObject(parent)
{
   cl_int code;
   _id = clCreateCommandQueue(context->id()
                              ,device->id()
                              ,CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE
                              ,&code);
   if ( code != CL_SUCCESS )
   {
      E_MAKE_EXCEPTION(e);
      fillException(&e,code);
      throw e;
   }
}






/*!
 */
CommandQueue::~CommandQueue()
{
   clReleaseCommandQueue(_id);
}






/*!
 */
cl_command_queue CommandQueue::id() const
{
   return _id;
}
