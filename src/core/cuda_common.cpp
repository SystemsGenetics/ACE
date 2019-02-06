#include "cuda_common.h"



//






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
