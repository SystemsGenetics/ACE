#include "eabstractanalytic.h"
//






/*!
 * This interface makes a new OpenCL object and returns its pointer. If this 
 * analytic type does not support OpenCL then a null pointer is returned. The 
 * default implementation returns a null pointer. 
 *
 * @return Pointer to new OpenCL object or null if this analytic does not support 
 *         OpenCL. 
 */
EAbstractAnalytic::OpenCL* EAbstractAnalytic::makeOpenCL() const
{
   return nullptr;
}






/*!
 * This interface initializes the analytic. This is called only once before any 
 * other interface is called for this analytic. The default implementation does 
 * nothing. 
 */
void EAbstractAnalytic::initialize()
{}






/*!
 * This interface is called to finish the analytic. This is called at the very end 
 * after every other interface call. The default implementation does nothing. 
 */
void EAbstractAnalytic::finish()
{}
