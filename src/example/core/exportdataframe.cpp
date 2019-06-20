#include "exportdataframe.h"
#include "exportdataframe_input.h"
#include "dataframe.h"
#include "dataframe_iterator.h"
#include "datafactory.h"



//






/*!
 * Return the total number of blocks this analytic must process as steps
 * or blocks of work. This implementation uses a work block for writing the
 * column names and a work block for writing each row.
 */
int ExportDataFrame::size() const
{
   EDEBUG_FUNC(this);

   return 1 + _in->rowSize();
}






/*!
 * Process the given index with a possible block of results if this analytic
 * produces work blocks. This implementation uses only the index of the result
 * block to determine which piece of work to do.
 *
 * @param result
 */
void ExportDataFrame::process(const EAbstractAnalyticBlock* result)
{
   EDEBUG_FUNC(this,result);

   // write the column names in the first step
   if ( result->index() == 0 )
   {
      // get column names
      EMetaArray columnNames {_in->columnNames()};

      // initialize output file stream
      _stream.setDevice(_out);
      _stream.setRealNumberPrecision(8);

      // write column names
      for ( int i = 0; i < _in->columnSize(); i++ )
      {
         _stream << columnNames.at(i).toString() << "\t";
      }
      _stream << "\n";
   }

   // write each row to the output file in a separate step
   else
   {
      // get row index
      int i = result->index() - 1;

      // get row name
      QString rowName {_in->rowNames().at(i).toString()};

      // load row from dataframe
      DataFrame::Iterator iterator(_in);
      iterator.read(i);

      // write row name
      _stream << rowName;

      // write row values
      for ( int j = 0; j < _in->columnSize(); j++ )
      {
         float value {iterator.at(j)};

         // if value is NAN use the NAN token
         if ( std::isnan(value) )
         {
            _stream << "\t" << _nanToken;
         }

         // else this is a normal floating point value
         else
         {
            _stream << "\t" << value;
         }
      }

      _stream << "\n";
   }

   // make sure writing output file worked
   if ( _stream.status() != QTextStream::Ok )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("File IO Error"));
      e.setDetails(tr("Qt Text Stream encountered an unknown error."));
      throw e;
   }
}






/*!
 * Make a new input object and return its pointer.
 */
EAbstractAnalyticInput* ExportDataFrame::makeInput()
{
   EDEBUG_FUNC(this);

   return new Input(this);
}






/*!
 * Initialize this analytic. This implementation checks to make sure the input
 * data object and output file have been set.
 */
void ExportDataFrame::initialize()
{
   EDEBUG_FUNC(this);

   if ( !_in || !_out )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Did not get valid input and/or output arguments."));
      throw e;
   }
}
