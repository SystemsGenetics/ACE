#ifndef DATAFRAME_H
#define DATAFRAME_H
#include <core/core.h>
//



/*!
 * This class implements the dataframe data object. A dataframe contains a
 * matrix of floating-point values, as well as metadata for row names and
 * column names. The matrix data can be accessed using the row interator.
 */
class DataFrame : public EAbstractData
{
   Q_OBJECT
public:
   class Iterator;
public:
   virtual qint64 dataEnd() const override final;
   virtual void readData() override final;
   virtual void writeNewData() override final;
   virtual void finish() override final;
   virtual QAbstractTableModel* model() override final;
public:
   qint32 rowSize() const;
   qint32 columnSize() const;
   EMetaArray rowNames() const;
   EMetaArray columnNames() const;
   void initialize(const QStringList& rowNames, const QStringList& columnNames);
   void initialize(const EMetaArray& rowNames, const EMetaArray& columnNames);
private:
   class Model;
private:
   void seekValue(int row, int column) const;
   /*!
    * The header size (in bytes) at the beginning of the file. The header
    * consists of the row size and the column size.
    */
   constexpr static const qint64 _headerSize {8};
   /*!
    * The number of rows in the dataframe.
    */
   qint32 _rowSize;
   /*!
    * The number of columns in the dataframe.
    */
   qint32 _columnSize;
   /*!
    * Pointer to a qt table model for this class.
    */
   Model* _model {nullptr};
};



#endif
