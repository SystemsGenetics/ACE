#include "mathtransform_block.h"
//






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






void* MathTransform::Block::operator new(size_t)
{
   return nullptr;
}






void MathTransform::Block::operator delete(void*)
{
}






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
