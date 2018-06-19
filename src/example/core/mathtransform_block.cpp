#include "mathtransform_block.h"
//






/*!
 * Constructs a new block with the given index. 
 *
 * @param index The index for this new block. 
 */
MathTransform::Block::Block(int index):
   EAbstractAnalytic::Block(index)
{}






/*!
 * Constructs a new block with the given index and integer. 
 *
 * @param index The index for this new block. 
 *
 * @param number The integer for this new block. 
 */
MathTransform::Block::Block(int index, qint32 number):
   EAbstractAnalytic::Block(index),
   _number(number)
{}






/*!
 * Writes this block's integer to the given data stream. 
 *
 * @param stream The data stream that is used to write out data. 
 */
void MathTransform::Block::write(QDataStream& stream) const
{
   stream << _number;
}






/*!
 * Reads in this block's integer from the given data stream. 
 *
 * @param stream The data stream that is used to read in data. 
 */
void MathTransform::Block::read(QDataStream& stream)
{
   stream >> _number;
}
