#include "cuda_common.h"






/*!
 * Get CUDA error name, if any.
 *
 * @param error
 */
QString CUDA::getErrorName(CUresult error)
{
   const char *str;
   cuGetErrorName(error, &str);

   return QString(str);
}






/*!
 * Get CUDA error description, if any.
 *
 * @param error
 */
QString CUDA::getErrorString(CUresult error)
{
   const char *str;
   cuGetErrorString(error, &str);

   return QString(str);
}






/*!
 * Get NVRTC error description, if any.
 *
 * @param result
 */
QString CUDA::getErrorString(nvrtcResult result)
{
   return nvrtcGetErrorString(result);
}
