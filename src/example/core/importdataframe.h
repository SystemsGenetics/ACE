#ifndef IMPORTDATAFRAME_H
#define IMPORTDATAFRAME_H
#include <core/core.h>
#include "global.h"
//



/*!
 * This class implements the import dataframe analytic. This analytic reads in
 * a text file which contains a matrix as a table; that is, with each row on a
 * line, each value separated by whitespace, and the first row and column
 * containing the row names and column names, respectively. Elements which have
 * the given NAN token are read in as NAN.
 */
class ImportDataFrame : public EAbstractAnalytic
{
   Q_OBJECT
public:
   class Input;
   virtual int size() const override final;
   virtual void process(const EAbstractAnalytic::Block* result) override final;
   virtual EAbstractAnalytic::Input* makeInput() override final;
   virtual void initialize();
private:
   /**
    * Structure used to load row data
    */
   struct Row
   {
      Row() = default;
      Row(int size)
      {
         values.resize(size);
      }

      QVector<float> values;
   };
   /**
    * Workspace variables to read from the input file.
    */
   QTextStream _stream;
   int _numLines {0};
   int _columnSize {0};
   QVector<Row> _rows;
   QStringList _rowNames;
   QStringList _columnNames;
   /*!
    * Pointer to the input text file.
    */
   QFile* _in {nullptr};
   /*!
    * Pointer to the output dataframe.
    */
   DataFrame* _out {nullptr};
   /*!
    * The string token used to represent NAN values.
    */
   QString _nanToken {"NA"};
};



#endif
