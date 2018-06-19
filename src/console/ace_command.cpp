#include "ace_command.h"



using namespace Ace;
//






/*!
 * Constructs a new command object, parsing arguments from the given command line 
 * variables. 
 *
 * @param argc The command line argument size passed from the main function. 
 *
 * @param argv The command line arguments passed from the main function. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Iterate through all given command line values except for the first for the 
 *    following steps. 
 *
 * 2. If the argument begins with "--" then skip this argument and the next 
 *    argument, else add this argument to this object's list of arguments. 
 */
Command::Command(int argc, char** argv)
{
   for (int i = 1; i < argc ;++i)
   {
      if ( argv[i][0] == '-' && argv[i][1] == '-' )
      {
         ++i;
      }
      else
      {
         _values << QString(argv[i]);
      }
   }
}






/*!
 * Returns the total number of arguments this object contains. 
 *
 * @return Total number of arguments this object contains. 
 */
int Command::size() const
{
   return _values.size();
}






/*!
 * Returns this object's argument at the given index. If the index is out of range 
 * qt terminates the program. 
 *
 * @param index The index of the argument that is returned. 
 *
 * @return Argument of this object at the given index. 
 */
QString Command::at(int index) const
{
   return _values.at(index);
}






/*!
 * Returns the first argument this object contains. If this object contains no 
 * arguments then qt terminates the program. 
 *
 * @return First argument this object contains. 
 */
QString Command::first() const
{
   return _values.first();
}






/*!
 * Pops the first argument this object contains off its list and returns its value. 
 * If this object contains no arguments then qt terminates the program. 
 */
QString Command::pop()
{
   return _values.takeFirst();
}






/*!
 * Pops the first argument this object contains off its list and returns the index 
 * where this argument's value is present in the given string list. If the 
 * argument's value is not present in the given string list then -1 is returned. If 
 * this object contains no arguments then qt terminates the program. 
 *
 * @param list  
 *
 * @return Index where this object's first argument was found in the given list or 
 *         -1 if it was not found. 
 */
int Command::pop(const QStringList& list)
{
   return list.indexOf(_values.takeFirst());
}






/*!
 * Returns the index where this object's first argument is present in the given 
 * string list. If the argument's value is not present in the given string list 
 * then -1 is returned. If this object contains no arguments then qt terminates the 
 * program. 
 *
 * @param list  
 *
 * @return Index where this object's first argument was found in the given list or 
 *         -1 if it was not found. 
 */
int Command::peek(const QStringList& list)
{
   return list.indexOf(_values.first());
}
