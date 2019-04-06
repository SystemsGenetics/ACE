#include "ace_analytic_abstractinput.h"
#include "eexception.h"
#include "eabstractanalyticblock.h"
#include "edebug.h"



using namespace std;
using namespace Ace::Analytic;
//






/*!
 * This interface saves the given result block to its underlying analytic and 
 * assumes the order of indexes given is not sorted and random. The default 
 * implementation of this interface uses this object's internal hopper to sort the 
 * given result blocks and calling the write result interface with the result 
 * blocks being ordered by index from least to greatest. 
 *
 * @param result The result block that is saved to the underlying analytic. 
 */
void AbstractInput::saveResult(std::unique_ptr<EAbstractAnalyticBlock>&& result)
{
   EDEBUG_FUNC(this,result.get())

   // If the given result block is a null pointer then make a new generic abstract 
   // analytic block with the next index expected, setting the given result block 
   // pointer to this new block. 
   if ( !result )
   {
      result.reset(new EAbstractAnalyticBlock(index()));
   }

   // If the given result block's index is the next index expected for the underlying 
   // analytic to maintain order of indexes then go to the next step, else add the 
   // result block to this object's hopper and exit. 
   if ( result->index() == index() )
   {
      // Call the write result interface passing along the given result block and 
      // continue passing result blocks from this object's hopper mapping while it 
      // possesses the next expected index. 
      writeResult(std::move(result));
      while ( _hopper.contains(index()) )
      {
         writeResult(unique_ptr<EAbstractAnalyticBlock>(_hopper.take(index())));
      }
   }
   else
   {
      EAbstractAnalyticBlock* block {result.release()};
      _hopper.insert(block->index(),block);
   }
}






/*!
 * Deletes all result blocks contained by this object's hopper. 
 */
AbstractInput::~AbstractInput()
{
   EDEBUG_FUNC(this)

   // Iterate through the list of result block pointers in this object's hopper 
   // container, deleting each one. 
   for (auto result: _hopper.values())
   {
      delete result;
   }
}






/*!
 * This interface returns the next expected result block index to maintain order of 
 * result blocks. The default implementation throws an exception as this must be 
 * implemented if the save result interface is not implemented. 
 *
 * @return The next expected result block index to maintain order. 
 */
int AbstractInput::index() const
{
   EDEBUG_FUNC(this)

   // Throw an exception. 
   E_MAKE_EXCEPTION(e);
   e.setTitle(QObject::tr("Logic Error"));
   e.setDetails(QObject::tr("Cannot call index interface without implementation."));
   throw e;
}






/*!
 * This interface is called to save the given result block to the underlying 
 * analytic and it can be assumed that the index order is maintained from least to 
 * greatest. The default implementation throws an exception as this must be 
 * implemented if the save result interface is not implemented. 
 *
 * @param result The result block that is saved to the underlying analytic. 
 */
void AbstractInput::writeResult(std::unique_ptr<EAbstractAnalyticBlock>&& result)
{
   EDEBUG_FUNC(this,result.get())

   // Delete the given result block and throw and exception. 
   result.reset();
   E_MAKE_EXCEPTION(e);
   e.setTitle(QObject::tr("Logic Error"));
   e.setDetails(QObject::tr("Cannot call write result interface without implementation."));
   throw e;
}
