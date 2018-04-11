#include "mathtransform_serial.h"
#include "mathtransform_block.h"



using namespace std;
//






/*!
 * Constructs a new serial object with the given math transform as its parent. 
 *
 * @param parent The parent math transform object for this new serial object. 
 */
MathTransform::Serial::Serial(MathTransform* parent):
   EAbstractAnalytic::Serial(parent),
   _base(parent)
{}






/*!
 * Implements the interface that reads in the given work block and saves the 
 * results in a new results block. This implementation takes the input number and 
 * does the required work transform on it. 
 *
 * @param block Pointer to work block that is used to produce the returned results 
 *              block. 
 *
 * @return Pointer to results block produced from the given work block. 
 */
std::unique_ptr<EAbstractAnalytic::Block> MathTransform::Serial::execute(const EAbstractAnalytic::Block* block)
{
   const Block* valid {block->cast<Block>()};
   qint32 value {valid->_number};
   switch (_base->_type)
   {
   case Operation::Addition:
      value += _base->_amount;
      break;
   case Operation::Subtraction:
      value -= _base->_amount;
      break;
   case Operation::Multiplication:
      value *= _base->_amount;
      break;
   case Operation::Division:
      value /= _base->_amount;
      break;
   }
   return unique_ptr<EAbstractAnalytic::Block>(new Block(value));
}
