#include "mathtransform.h"
#include "mathtransform_block.h"
#include "mathtransform_input.h"
#include "mathtransform_serial.h"
#include "integerarray.h"



using namespace std;
//






/*!
 * Implements the interface that returns the total number of blocks this analytic 
 * must process as blocks of work. 
 *
 * @return Total number of blocks that this analytic must work on. 
 */
int MathTransform::size() const
{
   return _in->_numbers.size();
}






/*!
 * Implements the interface that makes a new block of work from the given block 
 * index. 
 *
 * @param index Index used to make the block of work. 
 *
 * @return Pointer to block of work or null if this analytic has no work blocks. 
 */
std::unique_ptr<EAbstractAnalytic::Block> MathTransform::makeBlock(int index) const
{
   return unique_ptr<EAbstractAnalytic::Block>(new Block(_in->_numbers.at(index)));
}






/*!
 * Implements the interface that reads in a block of results made from a block of 
 * work with the corresponding index. 
 *
 * @param index  
 *
 * @param results  
 */
void MathTransform::process(int index, const EAbstractAnalytic::Block* results)
{
   Q_UNUSED(index)
   const Block* valid {results->cast<Block>()};
   _out->_numbers << valid->_number;
}






/*!
 * Implements the interface that makes a new input object and returns its pointer. 
 *
 * @return Pointer to new input object. 
 */
EAbstractAnalytic::Input* MathTransform::makeInput()
{
   return new Input(this);
}






/*!
 * Implements the interface that makes a new serial object and returns its pointer. 
 *
 * @return Pointer to new serial object. 
 */
EAbstractAnalytic::Serial* MathTransform::makeSerial()
{
   return new Serial(this);
}






/*!
 * Implements the interface that initializes this analytic. This implementation 
 * checks to make sure it has valid input and output data objects. 
 */
void MathTransform::initialize()
{
   if ( !_in || !_out )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("The required input and/or output data objects were not set."));
      throw e;
   }
}
