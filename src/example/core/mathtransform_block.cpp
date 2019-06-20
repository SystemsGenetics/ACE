#include "mathtransform_block.h"



//






/*!
 * Constructs a new block with the given index. 
 *
 * @param index The index for this new block. 
 */
MathTransform::Block::Block(int index):
   EAbstractAnalyticBlock(index)
{}






/*!
 * Constructs a new block with the given index and data. 
 *
 * @param index The index for this new block. 
 *
 * @param size The size for this new block. 
 *
 * @param data Pointer to the data for this new block. 
 */
MathTransform::Block::Block(int index, int size, const float* data):
   EAbstractAnalyticBlock(index)
{
   _data.resize(size);
   
   for ( int i = 0; i < size; i++ )
   {
      _data[i] = data[i];
   }
}






/*!
 * Writes this block's data to the given data stream. 
 *
 * @param stream The data stream that is used to write out data. 
 */
void MathTransform::Block::write(QDataStream& stream) const
{
   stream << _data;
}






/*!
 * Reads in this block's data from the given data stream. 
 *
 * @param stream The data stream that is used to read in data. 
 */
void MathTransform::Block::read(QDataStream& stream)
{
   stream >> _data;
}
