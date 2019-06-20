#include "mathtransform_serial.h"
#include "mathtransform_block.h"
#include "core/elog.h"



using namespace std;
//






/*!
 * Constructs a new serial object with the given math transform as its parent. 
 *
 * @param parent The parent math transform object for this new serial object. 
 */
MathTransform::Serial::Serial(MathTransform* parent):
   EAbstractAnalyticSerial(parent),
   _base(parent)
{}






/*!
 * Implements the interface that reads in the given work block and saves the 
 * results in a new results block. This implementation takes the input row and 
 * does the required work transform on it. 
 *
 * @param block Pointer to work block that is used to produce the returned results 
 *              block. 
 *
 * @return Pointer to results block produced from the given work block. 
 */
std::unique_ptr<EAbstractAnalyticBlock> MathTransform::Serial::execute(const EAbstractAnalyticBlock* block)
{
   if ( ELog::isActive() )
   {
      ELog() << tr("Executing(serial) work index %1.").arg(block->index());
   }
   // Cast the given work block to this analytic block type and get the row data
   // from it into _row_. 
   const Block* valid {block->cast<Block>()};
   QVector<float> row {valid->_data};

   // Transform _row_ based off this object's parent analytic object's operation 
   // type and amount. 
   for (float& value : row)
   {
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
   }

   // Return a new result block with the given work block index and the transformed 
   // _row_. 
   return unique_ptr<EAbstractAnalyticBlock>(new Block(block->index(),row.size(),row.data()));
}
