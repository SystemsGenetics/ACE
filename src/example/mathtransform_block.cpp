#include "mathtransform_block.h"
//






/*!
 *
 * @param index  
 */
MathTransform::Block::Block(int index):
   EAbstractAnalytic::Block(index)
{}






/*!
 *
 * @param index  
 *
 * @param number  
 */
MathTransform::Block::Block(int index, qint32 number):
   EAbstractAnalytic::Block(index),
   _number(number)
{}






/*!
 *
 * @param stream  
 */
void MathTransform::Block::write(QDataStream& stream) const
{
   stream << _number;
}






/*!
 *
 * @param stream  
 */
void MathTransform::Block::read(QDataStream& stream)
{
   stream >> _number;
}
