#include "eabstractanalytic.h"
#include "eabstractanalytic_block.h"
#include "ace_qmpi.h"
#include "edebug.h"



//






/*!
 * This interface creates and returns a work block for this analytic with the given 
 * index. The default implementation throws an exception because this should never 
 * be called if it is not implemented by an analytic. 
 *
 * @param index Index used to make the block of work. 
 *
 * @return Pointer new work block with the given index. 
 */
std::unique_ptr<EAbstractAnalytic::Block> EAbstractAnalytic::makeWork(int index) const
{
   EDEBUG_FUNC(this,index)

   // Throw an exception. 
   Q_UNUSED(index)
   E_MAKE_EXCEPTION(e);
   e.setTitle(tr("Logic Error"));
   e.setDetails(tr("Attempting to make an abstract analytic block with a simple analytic."));
   throw e;
}






/*!
 * This interface creates an empty and uninitialized work block. This is used by 
 * MPI and chunk runs for reading in work blocks from byte arrays. The default 
 * implementation throws an exception because this should never be called if it is 
 * not implemented by an analytic. 
 *
 * @return Pointer uninitialized work block. 
 */
std::unique_ptr<EAbstractAnalytic::Block> EAbstractAnalytic::makeWork() const
{
   EDEBUG_FUNC(this)

   // Throw an exception. 
   E_MAKE_EXCEPTION(e);
   e.setTitle(tr("Logic Error"));
   e.setDetails(tr("Attempting to make an abstract analytic block with a simple analytic."));
   throw e;
}






/*!
 * This interface creates an empty and uninitialized result block. This is used by 
 * MPI and chunk runs for reading in work blocks from byte arrays. The default 
 * implementation throws an exception because this should never be called if it is 
 * not implemented by an analytic. 
 *
 * @return Pointer uninitialized result block. 
 */
std::unique_ptr<EAbstractAnalytic::Block> EAbstractAnalytic::makeResult() const
{
   EDEBUG_FUNC(this)

   // Throw an exception. 
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
   EDEBUG_FUNC(this)

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
   EDEBUG_FUNC(this)

   return nullptr;
}






/*!
 * This interface makes a new CUDA object and returns its pointer. If this
 * analytic type does not support CUDA then a null pointer is returned. The
 * default implementation returns a null pointer.
 *
 * @return Pointer to new CUDA object or null if this analytic does not support
 *         CUDA.
 */
EAbstractAnalytic::CUDA* EAbstractAnalytic::makeCUDA()
{
   EDEBUG_FUNC(this)

   return nullptr;
}






/*!
 * This interface initializes this analytic. This is called only once before any 
 * other interface is called for this analytic. The default implementation does 
 * nothing. 
 */
void EAbstractAnalytic::initialize()
{
   EDEBUG_FUNC(this)
}






/*!
 * This interface initializes any outputs for this analytic. This is called only 
 * once before any other interface is called for this analytic. This is only called 
 * for analytic objects that will process output result blocks. The default 
 * implementation does nothing. 
 */
void EAbstractAnalytic::initializeOutputs()
{
   EDEBUG_FUNC(this)
}






/*!
 * This interface is called to finish this analytic. This is called at the very end 
 * after every other interface call. The default implementation does nothing. 
 */
void EAbstractAnalytic::finish()
{
   EDEBUG_FUNC(this)
}






/*!
 * Tests if this is the master node in an MPI run. If this is not an MPI run then 
 * this will also return true. 
 *
 * @return True if this is the master node of an MPI run or this is not running in 
 *         MPI mode, otherwise false is returned. 
 */
bool EAbstractAnalytic::isMaster()
{
   EDEBUG_FUNC()

   return Ace::QMPI::instance().isMaster();
}
