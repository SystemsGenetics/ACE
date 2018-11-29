#ifndef EXPORTDATAFRAME_H
#define EXPORTDATAFRAME_H
#include <core/core.h>
#include "global.h"
//



/*!
 * This class implements the export dataframe analytic. This analytic writes an
 * dataframe to a text file as table; that is, with each row on a line, each
 * value separated by whitespace, and the first row and column containing the
 * row names and column names, respectively. Elements which are NAN in the
 * dataframe are written as the given NAN token.
 */
class ExportDataFrame : public EAbstractAnalytic
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
    * Workspace variables to write to the output file
    */
   QTextStream _stream;
   /*!
    * Pointer to the input dataframe.
    */
   DataFrame* _in {nullptr};
   /*!
    * Pointer to the output text file.
    */
   QFile* _out {nullptr};
   /*!
    * The string token used to represent NAN values.
    */
   QString _nanToken {"NA"};
};



#endif
