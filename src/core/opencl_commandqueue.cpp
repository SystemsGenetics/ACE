#include "opencl_commandqueue.h"
#include "opencl_context.h"
#include "opencl_device.h"
#include "opencl_common.h"
#include "eexception.h"



using namespace OpenCL;
//






/*!
 * Constructs a new command queue with the given context, device, and optional 
 * parent. If the given device is not part of the given context then an exception 
 * is thrown. 
 *
 * @param context Pointer to context this command queue is created from. 
 *
 * @param device Pointer to device this command queue is created from. 
 *
 * @param parent Optional parent for this new command queue. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a new OpenCL command queue with the given context and device, setting 
 *    it to out of order execution and saving the new ID to this object. If 
 *    creation fails then throw an exception. 
 */
CommandQueue::CommandQueue(Context* context, Device* device, QObject* parent):
   QObject(parent),
   _device(device)
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
 * Releases the underlying OpenCL command queue resource. 
 */
CommandQueue::~CommandQueue()
{
   clReleaseCommandQueue(_id);
}






/*!
 * Returns the OpenCL command queue ID of this object. 
 *
 * @return OpenCL command queue ID of this object. 
 */
cl_command_queue CommandQueue::id() const
{
   return _id;
}






/*!
 * Returns a pointer to the device that this command queue uses. 
 *
 * @return Pointer to device for this command queue. 
 */
Device* CommandQueue::device() const
{
   return _device;
}
