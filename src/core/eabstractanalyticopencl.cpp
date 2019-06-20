#include "eabstractanalyticopencl.h"
#include "edebug.h"






/*!
 * Constructs a new abstract OpenCL object with the given parent.
 *
 * @param parent The parent analytic object for this new OpenCL object.
 */
EAbstractAnalyticOpenCL::EAbstractAnalyticOpenCL(EAbstractAnalytic* parent)
   :
   QObject(parent)
{
   EDEBUG_FUNC(this,parent);
}

