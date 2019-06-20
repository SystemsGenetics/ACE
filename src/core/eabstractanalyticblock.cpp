#include "eabstractanalyticblock.h"
#include "edebug.h"






/*!
 * Extracts the index from a byte array created from a to bytes method call of
 * this class. This is used to extract special codes used in MPI.
 *
 *
 * @return Index or special code of block byte array.
 */
int EAbstractAnalyticBlock::extractIndex(const QByteArray& data)
{
   // Add the debug header.
   EDEBUG_FUNC(data);

   // Read the index from the byte array, making sure it worked.
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

   // Return the extracted index.
   return ret;
}






/*!
 * Constructs a new block with the given index.
 *
 * @param index The index for this new block.
 */
EAbstractAnalyticBlock::EAbstractAnalyticBlock(int index)
   :
   _index(index)
{
   EDEBUG_FUNC(this,index);
}






/*!
 * Returns the index of this block.
 *
 * @return Index of this block.
 */
int EAbstractAnalyticBlock::index() const
{
   EDEBUG_FUNC(this);
   return _index;
}






/*!
 * Returns a byte array that contains all this blocks information. This includes
 * any implementation's data.
 *
 * @return Byte array containing all data of this block.
 */
QByteArray EAbstractAnalyticBlock::toBytes() const
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Create a new byte array and a data stream for writing to it.
   QByteArray ret;
   QDataStream stream(&ret,QIODevice::WriteOnly);

   // Write this block's index and and then this block's data by calling the write
   // interface.
   stream << _index;
   write(stream);

   // Make sure all writing to the byte array was successful.
   if ( stream.status() != QDataStream::Ok )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Read Error"));
      e.setDetails(tr("Failed reading index from byte array."));
      throw e;
   }

   // Return the byte array containing this block's index and data.
   return ret;
}






/*!
 * Read in new data from the given byte array for this block, overwriting any
 * previous data, including the data of this block's implementation.
 *
 * @param data The byte array that is read in and should contain data for an
 *             index and data of a block.
 */
void EAbstractAnalyticBlock::fromBytes(const QByteArray& data)
{
   // Add the debug header.
   EDEBUG_FUNC(this,data);

   // Create a data stream for reading the given byte array.
   QDataStream stream(data);

   // Read in this block's index and then its data by calling the read interface,
   // making sure all reading was successful.
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
void EAbstractAnalyticBlock::write(QDataStream& stream) const
{
   EDEBUG_FUNC(this,&stream);
   Q_UNUSED(stream)
}






/*!
 * This interface reads in new data from the given byte array, overwriting any
 * data it previously contained. The default implementation does nothing.
 *
 * @param stream The data stream that is used to read in this block's
 *               implementation data.
 */
void EAbstractAnalyticBlock::read(QDataStream& stream)
{
   EDEBUG_FUNC(this,&stream);
   Q_UNUSED(stream)
}

