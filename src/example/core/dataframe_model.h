#ifndef DATAFRAME_MODEL_H
#define DATAFRAME_MODEL_H
#include "dataframe.h"
//



/*!
 * This class implements the qt table model for the dataframe data object,
 * which represents the dataframe as a table.
 */
class DataFrame::Model : public QAbstractTableModel
{
public:
   Model(DataFrame* matrix);
   virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override final;
   virtual int rowCount(const QModelIndex& parent) const override final;
   virtual int columnCount(const QModelIndex& parent) const override final;
   virtual QVariant data(const QModelIndex& index, int role) const override final;
private:
   /*!
    * Pointer to the data object for this table model.
    */
   DataFrame* _matrix;
};



#endif
