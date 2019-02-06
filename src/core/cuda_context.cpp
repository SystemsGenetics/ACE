#include "cuda_context.h"
#include "cuda_device.h"



using namespace CUDA;
//






/*!
 * Constructs a new CUDA context with the given device and optional parent.
 *
 * @param device Pointer to the device from which this context is created.
 *
 * @param parent Optional parent for this new context. 
 */
Context::Context(Device* device, QObject* parent):
   QObject(parent)
{
   // Create a new CUDA context with the given device, setting this object's CUDA
   // context ID to the one returned. If creating the context fails then throw
   // an exception.
   CUDA_SAFE_CALL(cuCtxCreate(&_id, 0, device->id()));
}






/*!
 * Releases the underlying CUDA context that this object represents.
 */
Context::~Context()
{
   cuCtxDestroy(_id);
}






/*!
 * Binds the CUDA context to the current thread.
 */
void Context::setCurrent() const
{
   CUDA_SAFE_CALL(cuCtxSetCurrent(_id));
}
