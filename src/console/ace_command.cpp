#include "ace_command.h"



using namespace Ace;
//






/*!
 *
 * @param argc  
 *
 * @param argv  
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
 */
int Command::size() const
{
   return _values.size();
}






/*!
 *
 * @param index  
 */
QString Command::at(int index) const
{
   return _values.at(index);
}






/*!
 */
QString Command::first() const
{
   return _values.first();
}






/*!
 */
QString Command::pop()
{
   return _values.takeFirst();
}






/*!
 *
 * @param list  
 */
int Command::pop(const QStringList& list)
{
   return list.indexOf(_values.takeFirst());
}
