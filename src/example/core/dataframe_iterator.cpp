#include "dataframe_iterator.h"
//






/*!
 * Return the value at the given index.
 *
 * @param index
 */
float& DataFrame::Iterator::operator[](int index)
{
   EDEBUG_FUNC(this,index);

   // make sure the index is valid
   if ( index < 0 || index >= _matrix->_columnSize )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Domain Error"));
      e.setDetails(tr("Attempting to access column %1 when maximum is %2.").arg(index)
                   .arg(_matrix->_columnSize-1));
      throw e;
   }

   // return the specified value
   return _data[index];
}






/*!
 * Construct a row iterator for an dataframe. Additionally, if the
 * matrix is already initialized, read the first row into memory.
 *
 * @param matrix
 * @param isInitialized
 */
DataFrame::Iterator::Iterator(DataFrame* matrix, bool isInitialized):
   _matrix(matrix),
   _data(new float[matrix->columnSize()])
{
   EDEBUG_FUNC(this,matrix,isInitialized);

   if ( isInitialized )
   {
      read(_index);
   }
}






/*!
 * Destruct a row iterator.
 */
DataFrame::Iterator::~Iterator()
{
   EDEBUG_FUNC(this);

   delete[] _data;
}






/*!
 * Read a row of the dataframe from the data object file into memory.
 *
 * @param index
 */
void DataFrame::Iterator::read(int index)
{
   EDEBUG_FUNC(this,index);

   // make sure the index is valid
   if ( index < 0 || index >= _matrix->_rowSize )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Domain Error"));
      e.setDetails(tr("Attempting to read row %1 when maximum is %2.").arg(index)
                   .arg(_matrix->_rowSize-1));
      throw e;
   }

   // seek to the beginning of the specified row in the data object file
   _matrix->seekValue(index,0);

   // read the entire row into memory
   for (int i = 0; i < _matrix->columnSize() ;++i)
   {
      _matrix->stream() >> _data[i];
   }

   // set the iterator's current index
   _index = index;
}






/*!
 * Read the next row of the dataframe into memory.
 */
bool DataFrame::Iterator::readNext()
{
   EDEBUG_FUNC(this);

   // make sure that there is another row in the dataframe
   if ( (_index + 1) >= _matrix->_rowSize )
   {
      return false;
   }

   // read the next row
   read(_index + 1);

   // return success
   return true;
}






/*!
 * Write the iterator's row data to the data object file corresponding to
 * the given row index.
 *
 * @param index
 */
void DataFrame::Iterator::write(int index)
{
   EDEBUG_FUNC(this,index);

   // make sure the index is valid
   if ( index < 0 || index >= _matrix->_rowSize )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Domain Error"));
      e.setDetails(tr("Attempting to write row %1 when maximum is %2.").arg(index)
                   .arg(_matrix->_rowSize-1));
      throw e;
   }

   // seek to the beginning of the specified row in the data object file
   _matrix->seekValue(index,0);

   // write the entire row to the data object
   for (int i = 0; i < _matrix->columnSize() ;++i)
   {
      _matrix->stream() << _data[i];
   }

   // set the iterator's current index
   _index = index;
}






/*!
 * Write the iterator's row data to the next row in the data object file.
 */
bool DataFrame::Iterator::writeNext()
{
   EDEBUG_FUNC(this);

   // make sure there is another row in the dataframe
   if ( (_index + 1) >= _matrix->_rowSize )
   {
      return false;
   }

   // write to the next row
   write(_index + 1);

   // return success
   return true;
}






/*!
 * Return the value at the given index.
 *
 * @param index
 */
float DataFrame::Iterator::at(int index) const
{
   EDEBUG_FUNC(this,index);

   // make sure the index is valid
   if ( index < 0 || index >= _matrix->_columnSize )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Domain Error"));
      e.setDetails(tr("Attempting to access column %1 when maximum is %2.").arg(index)
                   .arg(_matrix->_columnSize-1));
      throw e;
   }

   // return the specified value
   return _data[index];
}
