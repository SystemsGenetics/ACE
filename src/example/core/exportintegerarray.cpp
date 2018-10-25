#include "exportintegerarray.h"
#include "exportintegerarray_input.h"
#include "integerarray.h"
#include "datafactory.h"



//






/*!
 * Deletes the qt text stream used to write out to the output text file. 
 */
ExportIntegerArray::~ExportIntegerArray()
{
   delete _stream;
}






/*!
 * Implements the interface that returns the total number of blocks this analytic 
 * must process as steps or blocks of work. 
 *
 * @return Total number of blocks or steps that this analytic must work on. 
 */
int ExportIntegerArray::size() const
{
   return _in->_numbers.size();
}






/*!
 * Implements the interface that processes the given index with a possible block of 
 * results if this analytic produces work blocks. This analytic implementation has 
 * no work blocks. 
 *
 * @param result Pointer to the block of results that is read in. 
 */
void ExportIntegerArray::process(const EAbstractAnalytic::Block* result)
{
   // Write out the integer value at the index of the given result block from the 
   // input integer array. If this is not the last integer value that will be written 
   // then also write out one space. 
   *_stream << _in->_numbers.at(result->index());
   if ( result->index() != (_in->_numbers.size() - 1) )
   {
      *_stream << " ";
   }
}






/*!
 * Implements the interface that makes a new input object and returns its pointer. 
 *
 * @return Pointer to new input object. 
 */
EAbstractAnalytic::Input* ExportIntegerArray::makeInput()
{
   return new Input(this);
}






/*!
 * Implements the interface that initializes this analytic. This implementation 
 * checks to make sure the input file and output data object has been set. 
 */
void ExportIntegerArray::initialize()
{
   // If the input integer array or output text file is not set then throw an 
   // exception. 
   if ( !_in || !_out )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("Did not get valid input and/or output arguments."));
      throw e;
   }

   // Create a new qt text stream attached to the output text file and save its 
   // pointer to this object. 
   _stream = new QTextStream(_out);
}
