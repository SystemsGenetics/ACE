#include "ace_options.h"



using namespace Ace;
//






/*!
 * Constructs a new options object parsing options from the given command line 
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
 * 2. If the argument begins with "--" and there is an argument after this one then 
 *    take both and insert them into this option's mapping with the first argument, 
 *    minus two beginning dashes, as the key and the second argument the value. 
 *    Skip the second argument moving to the next one. 
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
 * Returns the total number of options this object contains. 
 *
 * @return Total number of options this object contains. 
 */
int Options::size() const
{
   return _map.size();
}






/*!
 * Returns the key at the given index of this object's mapping. If the index is out 
 * of range qt terminates the program. 
 *
 * @param index  
 *
 * @return Key at the given index of this object's mapping. 
 */
QString Options::key(int index) const
{
   return _map.keys().at(index);
}






/*!
 * Returns the value at the given index of this object's mapping. If the index is 
 * out of range qt terminates the program. 
 *
 * @param index  
 *
 * @return Index at he given index of this object's mapping. 
 */
QString Options::value(int index) const
{
   return _map.values().at(index);
}






/*!
 * Returns the value mapped with the given key in this object's mapping. If no 
 * value exists with the given value an empty string is returned. 
 *
 * @param key  
 *
 * @return Value mapped with the given key in this object's mapping or an empty 
 *         string if no value is found with the given key. 
 */
QString Options::find(const QString& key) const
{
   return _map.value(key);
}
