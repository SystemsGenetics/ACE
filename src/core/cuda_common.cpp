#include "cuda_common.h"



//






void CUDA::throwError(EException *e, CUresult error)
{
   e->setTitle(QObject::tr("CUDA Error"));
   e->setDetails(QObject::tr("%1: %2.").arg(getErrorName(error)).arg(getErrorString(error)));
   throw e;
}






void CUDA::throwError(EException *e, nvrtcResult error)
{
   e->setTitle(QObject::tr("CUDA Error"));
   e->setDetails(QObject::tr("%1: %2.").arg("NVRTC Error").arg(getErrorString(error)));
   throw e;
}






/*!
 * Get the name of a CUDA error code.
 *
 * @param error CUDA error code.
 *
 * @return The name of the given error code.
 */
QString CUDA::getErrorName(CUresult error)
{
   const char *str;
   cuGetErrorName(error, &str);

   return QString(str);
}






/*!
 * Get the description of a CUDA error code.
 *
 * @param error CUDA error code.
 *
 * @return The description of the given error code.
 */
QString CUDA::getErrorString(CUresult error)
{
   const char *str;
   cuGetErrorString(error, &str);

   return QString(str);
}






/*!
 * Get the description of a NVRTC error code.
 *
 * @param error NVRTC error code.
 *
 * @return The description of the given error code.
 */
QString CUDA::getErrorString(nvrtcResult error)
{
   return nvrtcGetErrorString(error);
}
