#include "eabstractanalytic.h"
#include "eabstractanalytic_block.h"
//






/*!
 *
 * @param index Index used to make the block of work. 
 */
std::unique_ptr<EAbstractAnalytic::Block> EAbstractAnalytic::makeWork(int index) const
{
   Q_UNUSED(index)
   E_MAKE_EXCEPTION(e);
   e.setTitle(tr("Logic Error"));
   e.setDetails(tr("Attempting to make an abstract analytic block with a simple analytic."));
   throw e;
}






/*!
 */
std::unique_ptr<EAbstractAnalytic::Block> EAbstractAnalytic::makeWork() const
{
   E_MAKE_EXCEPTION(e);
   e.setTitle(tr("Logic Error"));
   e.setDetails(tr("Attempting to make an abstract analytic block with a simple analytic."));
   throw e;
}






/*!
 */
std::unique_ptr<EAbstractAnalytic::Block> EAbstractAnalytic::makeResult() const
{
   E_MAKE_EXCEPTION(e);
   e.setTitle(tr("Logic Error"));
   e.setDetails(tr("Attempting to make an abstract analytic block with a simple analytic."));
   throw e;
}






/*!
 * This interface makes a new serial object and returns its pointer. If this 
 * analytic type does not support serial then a null pointer is returned. The 
 * default implementation returns a null pointer. 
 *
 * @return Pointer to new serial object or null if no serial support. 
 */
EAbstractAnalytic::Serial* EAbstractAnalytic::makeSerial()
{
   return nullptr;
}






/*!
 * This interface makes a new OpenCL object and returns its pointer. If this 
 * analytic type does not support OpenCL then a null pointer is returned. The 
 * default implementation returns a null pointer. 
 *
 * @return Pointer to new OpenCL object or null if this analytic does not support 
 *         OpenCL. 
 */
EAbstractAnalytic::OpenCL* EAbstractAnalytic::makeOpenCL()
{
   return nullptr;
}






/*!
 * This interface initializes this analytic. This is called only once before any 
 * other interface is called for this analytic. The default implementation does 
 * nothing. 
 */
void EAbstractAnalytic::initialize()
{}






/*!
 * This interface is called to finish this analytic. This is called at the very end 
 * after every other interface call. The default implementation does nothing. 
 */
void EAbstractAnalytic::finish()
{}
