#include "eabstractanalytic_block.h"
//






/*!
 *
 * @param index  
 */
EAbstractAnalytic::Block::Block(int index):
   _index(index)
{}






/*!
 */
int EAbstractAnalytic::Block::index() const
{
   return _index;
}






/*!
 */
QByteArray EAbstractAnalytic::Block::toBytes() const
{
   QByteArray ret;
   QDataStream stream(&ret,QIODevice::WriteOnly);
   stream << _index;
   write(stream);
   return ret;
}






/*!
 *
 * @param data  
 */
void EAbstractAnalytic::Block::fromBytes(const QByteArray& data)
{
   QDataStream stream(data);
   stream >> _index;
   read(stream);
}
