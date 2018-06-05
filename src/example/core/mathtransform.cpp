#include "mathtransform.h"
#include "mathtransform_block.h"
#include "mathtransform_input.h"
#include "mathtransform_serial.h"
#include "mathtransform_opencl.h"
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
 * Implements the interface that creates and returns a work block for this analytic 
 * with the given index. This implementation take a single integer from its input 
 * integer array and makes a work block from it. 
 *
 * @param index Index used to make the block of work. 
 *
 * @return Pointer new work block with the given index. 
 */
std::unique_ptr<EAbstractAnalytic::Block> MathTransform::makeWork(int index) const
{
   return unique_ptr<EAbstractAnalytic::Block>(new Block(index,_in->_numbers.at(index)));
}






/*!
 * Implements the interface that creates an empty and uninitialized work block. 
 *
 * @return Pointer uninitialized work block. 
 */
std::unique_ptr<EAbstractAnalytic::Block> MathTransform::makeWork() const
{
   return unique_ptr<EAbstractAnalytic::Block>(new Block);
}






/*!
 * Implements the interface that creates an empty and uninitialized result block. 
 *
 * @return Pointer uninitialized result block. 
 */
std::unique_ptr<EAbstractAnalytic::Block> MathTransform::makeResult() const
{
   return unique_ptr<EAbstractAnalytic::Block>(new Block);
}






/*!
 * Implements the interface that reads in a block of results made from a block of 
 * work with the corresponding index. This implementation takes the integer result 
 * and appends it to its output integer array. 
 *
 * @param result  
 *
 *
 * Steps of Operation: 
 *
 * 1. Cast the result block to this implementation's type and then append its 
 *    integer result to this object's output integer array. 
 */
void MathTransform::process(const EAbstractAnalytic::Block* result)
{
   // 1
   const Block* valid {result->cast<Block>()};
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
 * Implements the interface that makes a new OpenCL object and returns its pointer. 
 *
 * @return Pointer to new OpenCL object. 
 */
EAbstractAnalytic::OpenCL* MathTransform::makeOpenCL()
{
   return new OpenCL(this);
}






/*!
 * Implements the interface that initializes this analytic. This implementation 
 * checks to make sure it has a valid input data object. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this object does not have a valid input integer array then throw an 
 *    exception. 
 */
void MathTransform::initialize()
{
   // 1
   if ( !_in )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("The required input data object was not set."));
      throw e;
   }
}
