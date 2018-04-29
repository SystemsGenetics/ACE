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
 *
 * @param index Index used to make the block of work. 
 */
std::unique_ptr<EAbstractAnalytic::Block> MathTransform::makeWork(int index) const
{
   return unique_ptr<EAbstractAnalytic::Block>(new Block(index,_in->_numbers.at(index)));
}






/*!
 */
std::unique_ptr<EAbstractAnalytic::Block> MathTransform::makeWork() const
{
   return unique_ptr<EAbstractAnalytic::Block>(new Block);
}






/*!
 *
 * @param index Index used to make the block of work. 
 */
std::unique_ptr<EAbstractAnalytic::Block> MathTransform::makeResult(int index) const
{
   return unique_ptr<EAbstractAnalytic::Block>(new Block(index));
}






/*!
 */
std::unique_ptr<EAbstractAnalytic::Block> MathTransform::makeResult() const
{
   return unique_ptr<EAbstractAnalytic::Block>(new Block);
}






/*!
 * Implements the interface that reads in a block of results made from a block of 
 * work with the corresponding index. 
 *
 * @param result  
 */
void MathTransform::process(const EAbstractAnalytic::Block* result)
{
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
 */
EAbstractAnalytic::OpenCL* MathTransform::makeOpenCL()
{
   return new OpenCL(this);
}






/*!
 * Implements the interface that initializes this analytic. This implementation 
 * checks to make sure it has valid input and output data objects. 
 */
void MathTransform::initialize()
{
   if ( !_in )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("The required input data object was not set."));
      throw e;
   }
}
