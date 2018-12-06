#include "cuda_context.h"
#include "cuda_device.h"



using namespace CUDA;






/*!
 * Constructs a new CUDA context with the given device.
 *
 * @param device
 */
Context::Context(Device* device, QObject* parent):
   QObject(parent)
{
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
