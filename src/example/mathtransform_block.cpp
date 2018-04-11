#include "mathtransform_block.h"
//






/*!
 *
 * @param number  
 */
MathTransform::Block::Block(qint32 number):
   _number(number)
{}






/*!
 */
QByteArray MathTransform::Block::toBytes() const
{
   QByteArray ret;
   QDataStream stream(&ret,QIODevice::WriteOnly);
   stream << _number;
   return ret;
}






/*!
 *
 * @param data  
 */
void MathTransform::Block::fromBytes(const QByteArray& data)
{
   QDataStream stream(data);
   stream >> _number;
}
