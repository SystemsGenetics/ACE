#include "ace_options.h"



using namespace Ace;
//






/*!
 *
 * @param argc  
 *
 * @param argv  
 */
Options::Options(int argc, char** argv)
{
   for (int i = 1; i < argc ;++i)
   {
      if ( argv[i][0] == '-' && argv[i][1] == '-' )
      {
         ++i;
         if ( i < argc )
         {
            _map.insert(QString(&argv[i-1][2]),QString(argv[i]));
         }
      }
   }
}






/*!
 */
int Options::size() const
{
   return _map.size();
}






/*!
 *
 * @param index  
 */
QString Options::key(int index) const
{
   return _map.keys().at(index);
}






/*!
 *
 * @param index  
 */
QString Options::value(int index) const
{
   return _map.values().at(index);
}






/*!
 *
 * @param key  
 */
QString Options::find(const QString& key) const
{
   return _map.value(key);
}
