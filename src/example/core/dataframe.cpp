#include "dataframe.h"
#include "dataframe_model.h"



//






/*!
 * Implements the interface that returns the index within this data object after
 * the end of where the data is stored. Defined as the header size plus the size
 * of the matrix data.
 *
 * @return Index of the first byte in the data object after the data section ends.
 */
qint64 DataFrame::dataEnd() const
{
   EDEBUG_FUNC(this);

   return _headerSize + (qint64)_rowSize * (qint64)_columnSize * sizeof(float);
}






/*!
 * Implements the interface that reads in the data of an existing data object that
 * was just opened.
 */
void DataFrame::readData()
{
   EDEBUG_FUNC(this);

   // seek to the beginning of the data
   seek(0);

   // read the header
   stream() >> _rowSize >> _columnSize;
}






/*!
 * Implements the interface that is called to initialize this data object's data to
 * a null state.
 */
void DataFrame::writeNewData()
{
   EDEBUG_FUNC(this);

   // initialize metadata object
   setMeta(EMetaObject());

   // seek to the beginning of the data
   seek(0);

   // write the header
   stream() << _rowSize << _columnSize;
}






/*!
 * Implements the interface that finalizes this data object's data after the
 * analytic that created it has finished giving it new data.
 */
void DataFrame::finish()
{
   EDEBUG_FUNC(this);

   // seek to the beginning of the data
   seek(0);

   // write the header
   stream() << _rowSize << _columnSize;
}






/*!
 * Implements the interface that returns a qt table model that represents the data
 * of this data object as a table.
 *
 * @return Pointer to model that represents the data of this data object.
 */
QAbstractTableModel* DataFrame::model()
{
   EDEBUG_FUNC(this);

   if ( !_model )
   {
      _model = new Model(this);
   }
   return _model;
}






/*!
 * Return the number of rows in this dataframe.
 */
qint32 DataFrame::rowSize() const
{
   EDEBUG_FUNC(this);

   return _rowSize;
}






/*!
 * Return the number of columns in this dataframe.
 */
qint32 DataFrame::columnSize() const
{
   EDEBUG_FUNC(this);

   return _columnSize;
}






/*!
 * Return the list of row names in this dataframe.
 */
EMetaArray DataFrame::rowNames() const
{
   EDEBUG_FUNC(this);

   return meta().toObject().at("rows").toArray();
}






/*!
 * Return the list of column names in this dataframe.
 */
EMetaArray DataFrame::columnNames() const
{
   EDEBUG_FUNC(this);

   return meta().toObject().at("columns").toArray();
}






/*!
 * Initialize this dataframe with a list of row names and a list of
 * column names.
 *
 * @param rowNames
 * @param columnNames
 */
void DataFrame::initialize(const QStringList& rowNames, const QStringList& columnNames)
{
   EDEBUG_FUNC(this,&rowNames,&columnNames);

   // create a metadata array of row names
   EMetaArray metaRowNames;
   for ( auto& rowName : rowNames )
   {
      metaRowNames.append(rowName);
   }

   // create a metadata array of column names
   EMetaArray metaColumnNames;
   for ( auto& columnName : columnNames )
   {
      metaColumnNames.append(columnName);
   }

   // initialize this dataframe with the metadata arrays
   initialize(metaRowNames, metaColumnNames);
}






/*!
 * Initialize this dataframe with a list of row names and a list of
 * column names.
 *
 * @param rowNames
 * @param columnNames
 */
void DataFrame::initialize(const EMetaArray& rowNames, const EMetaArray& columnNames)
{
   EDEBUG_FUNC(this,&rowNames,&columnNames);

   // save the row names and column names to metadata
   EMetaObject metaObject {meta().toObject()};
   metaObject.insert("rows", rowNames);
   metaObject.insert("columns", columnNames);
   setMeta(metaObject);

   // initialize the row size and column size accordingly
   _rowSize = rowNames.size();
   _columnSize = columnNames.size();
}






/*!
 * Seek to a particular value in this dataframe given a row index
 * and a column index.
 *
 * @param row
 * @param column
 */
void DataFrame::seekValue(int row, int column) const
{
   EDEBUG_FUNC(this,row,column);

   // make sure that the indices are valid
   if ( row < 0 || row >= _rowSize || column < 0 || column >= _columnSize )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Invalid (row,column) index (%1,%2) with size of (%1,%2).")
         .arg(row)
         .arg(column)
         .arg(_rowSize)
         .arg(_columnSize));
      throw e;
   }

   // seek to the specified position in the data
   seek(_headerSize + ((qint64)row * (qint64)_columnSize + (qint64)column) * sizeof(float));
}
