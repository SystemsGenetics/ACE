#include "dataframe_model.h"
//






/*!
 * Construct a table model for an dataframe.
 *
 * @param matrix
 */
DataFrame::Model::Model(DataFrame* matrix):
   _matrix(matrix)
{
   EDEBUG_FUNC(this,matrix);

   setParent(matrix);
}






/*!
 * Return a header name for the table model using a given index and
 * orientation (row / column).
 *
 * @param section
 * @param orientation
 * @param role
 */
QVariant DataFrame::Model::headerData(int section, Qt::Orientation orientation, int role) const
{
   EDEBUG_FUNC(this,section,orientation,role);

   // make sure the role is valid
   if ( role != Qt::DisplayRole )
   {
      return QVariant();
   }

   // determine whether to return a row name or column name
   switch (orientation)
   {
   case Qt::Vertical:
   {
      // get row names
      EMetaArray rowNames {_matrix->rowNames()};

      // make sure the index is valid
      if ( section >= 0 && section < rowNames.size() )
      {
         // return the specified row name
         return rowNames.at(section).toString();
      }

      // otherwise return empty string
      return QVariant();
   }
   case Qt::Horizontal:
   {
      // get column names
      EMetaArray columns {_matrix->columnNames()};

      // make sure the index is valid
      if ( section >= 0 && section < columns.size() )
      {
         // return the specified column name
         return columns.at(section).toString();
      }

      // otherwise return empty string
      return QVariant();
   }
   default:
      // return empty string if orientation is not valid
      return QVariant();
   }
}






/*!
 * Return the number of rows in the table model.
 */
int DataFrame::Model::rowCount(const QModelIndex&) const
{
   EDEBUG_FUNC(this);

   return _matrix->_rowSize;
}






/*!
 * Return the number of columns in the table model.
 */
int DataFrame::Model::columnCount(const QModelIndex&) const
{
   EDEBUG_FUNC(this);

   return _matrix->_columnSize;
}






/*!
 * Return a data element in the table model using the given index.
 *
 * @param index
 * @param role
 */
QVariant DataFrame::Model::data(const QModelIndex& index, int role) const
{
   EDEBUG_FUNC(this,&index,role);

   // make sure the index and role are valid
   if ( !index.isValid() || role != Qt::DisplayRole )
   {
      return QVariant();
   }

   // make sure the index is within the bounds of the dataframe
   if ( index.row() >= _matrix->_rowSize || index.column() >= _matrix->_columnSize )
   {
      return QVariant();
   }

   // get the specified value from the dataframe
   float value;
   _matrix->seekValue(index.row(), index.column());
   _matrix->stream() >> value;

   // return the specified value
   return value;
}
