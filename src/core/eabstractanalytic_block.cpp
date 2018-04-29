#include "eabstractanalytic_block.h"
#include "eexception.h"
//






/*!
 *
 * @param data  
 */
int EAbstractAnalytic::Block::extractIndex(const QByteArray& data)
{
   int ret;
   QDataStream stream(data);
   stream >> ret;
   if ( stream.status() != QDataStream::Ok )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Read Error"));
      e.setDetails(tr("Failed reading index from byte array."));
      throw e;
   }
   return ret;
}






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
   if ( stream.status() != QDataStream::Ok )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Read Error"));
      e.setDetails(tr("Failed reading index from byte array."));
      throw e;
   }
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
   if ( stream.status() != QDataStream::Ok )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Read Error"));
      e.setDetails(tr("Failed reading index from byte array."));
      throw e;
   }
}






/*!
 *
 * @param stream  
 */
void EAbstractAnalytic::Block::write(QDataStream& stream) const
{
   Q_UNUSED(stream)
}






/*!
 *
 * @param stream  
 */
void EAbstractAnalytic::Block::read(QDataStream& stream)
{
   Q_UNUSED(stream)
}
