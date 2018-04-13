#include "eabstractanalytic.h"
#include "eabstractanalytic_block.h"
//






/*!
 * This interface makes a new block of work from the given block index. The order 
 * of indexes called will always be in order starting at 0. If null is returned 
 * that means working on blocks is skipped and the read block interface is called 
 * immediately. The default implementation returns a null pointer. 
 *
 * @param index Index used to make the block of work. 
 *
 * @return Pointer to block of work or null if this analytic has no work blocks. 
 */
std::unique_ptr<EAbstractAnalytic::Block> EAbstractAnalytic::makeBlock(int index) const
{
   Q_UNUSED(index)
   return nullptr;
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
