#include "eabstractanalytic_block.h"
#include "eexception.h"



//






/*!
 * Extracts the index from a byte array created from a to bytes method call of this 
 * class. This is used to extract special codes used in MPI. 
 *
 * @param data  
 *
 * @return Index or special code of block byte array. 
 */
int EAbstractAnalytic::Block::extractIndex(const QByteArray& data)
{
   EDEBUG_FUNC(data)

   // Read the index from the byte array and return it. If reading fails then throw 
   // an exception. 
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
 * Constructs a new block with the given index. 
 *
 * @param index The index for this new block. 
 */
EAbstractAnalytic::Block::Block(int index):
   _index(index)
{
   EDEBUG_FUNC(this,index)
}






/*!
 * Returns the index of this block. 
 *
 * @return Index of this block. 
 */
int EAbstractAnalytic::Block::index() const
{
   EDEBUG_FUNC(this)

   return _index;
}






/*!
 * Returns a byte array that contains all this blocks information. This includes 
 * any implementation's data. 
 *
 * @return Byte array containing all data of this block. 
 */
QByteArray EAbstractAnalytic::Block::toBytes() const
{
   EDEBUG_FUNC(this)

   // Create a new byte array and a data stream for writing to it, writing this 
   // blocks index and calling this block's write interface for its implementation to 
   // save its data. If any write error occurs then throw an exception, else return 
   // the created byte array. 
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
 * Read in new data from the given byte array for this block, overwriting any 
 * previous data, including the data of this block's implementation. 
 *
 * @param data  
 */
void EAbstractAnalytic::Block::fromBytes(const QByteArray& data)
{
   EDEBUG_FUNC(this,data)

   // Read in this block's index and call this block's read interface for its 
   // implementation to read its data. If any reading fails then throw an exception. 
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
 * This interface writes out its data to the given qt data stream. The default 
 * implementation does nothing. 
 *
 * @param stream The data stream that is used to write out this block's 
 *               implementation data. 
 */
void EAbstractAnalytic::Block::write(QDataStream& stream) const
{
   EDEBUG_FUNC(this,&stream)

   Q_UNUSED(stream)
}






/*!
 * This interface reads in new data from the given byte array, overwriting any data 
 * it previously contained. The default implementation does nothing. 
 *
 * @param stream The data stream that is used to read in this block's 
 *               implementation data. 
 */
void EAbstractAnalytic::Block::read(QDataStream& stream)
{
   EDEBUG_FUNC(this,&stream)

   Q_UNUSED(stream)
}
