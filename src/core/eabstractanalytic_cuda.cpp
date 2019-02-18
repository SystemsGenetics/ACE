#include "eabstractanalytic_cuda.h"
#include "edebug.h"



//






/*!
 * Constructs a new abstract CUDA object with the given parent.
 *
 * @param parent The parent analytic object for this new CUDA object.
 */
EAbstractAnalytic::CUDA::CUDA(EAbstractAnalytic* parent):
   QObject(parent)
{
   EDEBUG_FUNC(this,parent)
}
