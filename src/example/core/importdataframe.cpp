#include "importdataframe.h"
#include "importdataframe_input.h"
#include "datafactory.h"
#include "dataframe_iterator.h"



//






/*!
 * Return the total number of blocks this analytic must process as steps
 * or blocks of work. This implementation uses a work block for reading
 * each line of the input file, plus one work block to create the output
 * data object.
 */
int ImportDataFrame::size() const
{
   EDEBUG_FUNC(this);

   return _numLines + 1;
}






/*!
 * Process the given index with a possible block of results if this analytic
 * produces work blocks. This implementation uses only the index of the result
 * block to determine which piece of work to do.
 *
 * @param result
 */
void ImportDataFrame::process(const EAbstractAnalyticBlock* result)
{
   EDEBUG_FUNC(this, result);

   // read or create the column names in the first step
   if ( result->index() == 0 )
   {
      // seek to the beginning of the input file
      _stream.seek(0);

      // read column names from first line
      QString line = _stream.readLine();
      auto words = line.splitRef(QRegExp("\\s+"), QString::SkipEmptyParts);

      // parse the column names
      _columnSize = words.size();

      for ( auto& word : words )
      {
         _columnNames.append(word.toString());
      }

      // make sure reading input file worked
      if ( _stream.status() != QTextStream::Ok )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(tr("File IO Error"));
         e.setDetails(tr("Qt Text Stream encountered an unknown error."));
         throw e;
      }
   }

   // read each row from the input file in a separate step
   else if ( result->index() < _numLines )
   {
      // read a line from the input file
      QString line = _stream.readLine();
      auto words = line.splitRef(QRegExp("\\s+"), QString::SkipEmptyParts);

      // make sure the number of words matches expected column size
      if ( words.size() == _columnSize + 1 )
      {
         // read row from text file
         Row row(_columnSize);

         for ( int i = 1; i < words.size(); ++i )
         {
            // if word matches the nan token then set it as such
            if ( words.at(i) == _nanToken )
            {
               row.values[i-1] = NAN;
            }

            // else this is a normal floating point value
            else
            {
               // read in the floating point value
               bool ok;
               row.values[i-1] = words.at(i).toDouble(&ok);

               // make sure reading worked
               if ( !ok )
               {
                  E_MAKE_EXCEPTION(e);
                  e.setTitle(tr("Parsing Error"));
                  e.setDetails(tr("Failed to read value \"%1\" for row %2.")
                     .arg(words.at(i).toString())
                     .arg(words.at(0).toString()));
                  throw e;
               }
            }
         }

         // append row data and row name
         _rows.append(row);
         _rowNames.append(words.at(0).toString());
      }

      // otherwise throw an error
      else
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(tr("Parsing Error"));
         e.setDetails(tr("Encountered row with incorrect amount of fields. "
                         "Read in %1 fields when it should have been %2. Row name is %3.")
            .arg(words.size()-1)
            .arg(_columnSize)
            .arg(words.at(0).toString()));
         throw e;
      }

      // make sure reading input file worked
      if ( _stream.status() != QTextStream::Ok )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(tr("File IO Error"));
         e.setDetails(tr("Qt Text Stream encountered an unknown error."));
         throw e;
      }
   }

   // create the output data object in the final step
   else if ( result->index() == _numLines )
   {
      // initialize dataframe
      _out->initialize(_rowNames, _columnNames);

      // iterate through each row
      DataFrame::Iterator iterator(_out);

      for ( int i = 0; i < _out->rowSize(); ++i )
      {
         // save each row to dataframe
         for ( int j = 0; j < _out->columnSize(); ++j )
         {
            iterator[j] = _rows[i].values[j];
         }

         iterator.write(i);
      }
   }
}






/*!
 * Make a new input object and return its pointer.
 */
EAbstractAnalyticInput* ImportDataFrame::makeInput()
{
   EDEBUG_FUNC(this);

   return new Input(this);
}






/*!
 * Initialize this analytic. This implementation checks to make sure the input
 * file and output data object have been set.
 */
void ImportDataFrame::initialize()
{
   EDEBUG_FUNC(this);

   // make sure input/output arguments are valid
   if ( !_in || !_out )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Did not get valid input and/or output arguments."));
      throw e;
   }

   // initialize input file stream
   _stream.setDevice(_in);

   // count the number of lines in the input file
   _numLines = 0;

   while ( !_stream.atEnd() )
   {
      _stream.readLine();
      _numLines++;
   }

   // make sure reading input file worked
   if ( _stream.status() != QTextStream::Ok )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("File IO Error"));
      e.setDetails(tr("Qt Text Stream encountered an unknown error."));
      throw e;
   }
}
