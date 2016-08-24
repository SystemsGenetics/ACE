#include <regex>
#include "getopts.h"
namespace AccelCompEng
{



/// @brief Decomposes user line.
///
/// Decomposes the raw user input line given into commands and options, adding
/// them to this new object's lists.
///
/// @param raw User input line.
///
/// @exception InvalidSyntax An error occured in the syntax of the user line
/// while attempting to decompose it.
GetOpts::GetOpts(const string& raw):
   _orig(raw)
{
   static const char* f = __PRETTY_FUNCTION__;
   using regex = std::regex;
   using regtok = std::sregex_token_iterator;
   regex pat {"[\\s\\t]+"};
   regtok end {};
   for (regtok r {raw.begin(),raw.end(),pat,-1};r!=end;++r)
   {
      const string& i {*r};
      if (i.front()=='-')
      {
         string val;
         string key;
         auto x = i.find_first_not_of('-');
         auto y = i.find('=');
         if (y!=string::npos)
         {
            bool cond = y==i.rfind('=')&&y>x;
            assert<InvalidSyntax>(cond,f,__LINE__);
            val = i.substr(x,y-x);
            key = i.substr(++y);
         }
         else
         {
            val = i.substr(x);
         }
         _opts.push_back({std::move(val),std::move(key)});
      }
      else
      {
         _comms.push_back(i);
      }
   }
}



/// Get origional user input line for this object.
///
/// @return Origional user input.
const GetOpts::string& GetOpts::orig() const
{
   return _orig;
}



/// Get total number of commands left for this object.
///
/// @return Number of commands.
int GetOpts::com_size() const
{
   return _comms.size();
}



/// Tests if the commands list for this object is empty.
///
/// @return True if commands list is empty, else false.
bool GetOpts::com_empty() const
{
   return _comms.empty();
}



/// @brief Interrogate front command.
///
/// Interrogate the command at front of list for this object, comparing it to
/// initializer list of strings given seeing if it matches any of them.
///
/// @param commands List of values to match front of list command by.
/// @return 0 if no match is found in list of command values, else increment
/// into initializer list where match was found starting with 1 for beginning
/// of list.
int GetOpts::com_get(initlist commands)
{
   int ret = 0;
   int count = 0;
   if (!_comms.empty())
   {
      for (auto i:commands)
      {
         count++;
         if (i==_comms.front())
         {
            ret = count;
            break;
         }
      }
   }
   return ret;
}



/// Returns reference to string of command in front of list for this object.
///
/// @return Front of list command.
GetOpts::string& GetOpts::com_front()
{
   return _comms.front();
}



/// Remove the command at front of list of commands for this object.
void GetOpts::com_pop()
{
   if (!_comms.empty())
   {
      _comms.pop_front();
   }
}



/// Get total number of options left for this object.
///
/// @return Number of options.
int GetOpts::size() const
{
   return _opts.size();
}



/// Tests if the options list for this object is empty.
///
/// @return True if options list is empty, else false.
bool GetOpts::empty() const
{
   return _opts.empty();
}



/// Interrogate list of options to see if the key of any option matches the
/// value given, optionally removing all matches from list of options if found.
///
/// @param opt Key value to match against.
/// @param del True to remove all matching options, else false.
/// @return True if one or more matches found, else false.
bool GetOpts::has_opt(const string& opt, bool del)
{
   bool ret {false};
   for (auto i = _opts.begin();i!=_opts.end();)
   {
      if (i->first==opt)
      {
         ret = true;
         if (del)
         {
            i = _opts.erase(i);
         }
         else
         {
            ++i;
         }
      }
      else
      {
         ++i;
      }
   }
   return ret;
}



/// Gets iterator for options list that points at the beginning of list.
///
/// @return Start of list options iterator.
GetOpts::Iterator GetOpts::begin()
{
   return {_opts.begin()};
}



/// Gets iterator for options list that points to one past end of list.
///
/// @return One past end of list iterator.
GetOpts::Iterator GetOpts::end()
{
   return {_opts.end()};
}



/// Removes option from list by iterator given.
///
/// @param i Iterator that points to option that will be removed.
/// @return Iterator that points to next option in list or one past end of list
/// if the removed option was at back of list.
GetOpts::Iterator GetOpts::erase(Iterator i)
{
   return {_opts.erase(i._i)};
}



/// Get key value of this option.
///
/// @return Key value.
const GetOpts::string& GetOpts::Iterator::key() const
{
   return _i->first;
}



/// Tests if the given key value matches the key value of this option.
///
/// @return True if they match, else fail.
bool GetOpts::Iterator::is_key(const string& cmp) const
{
   return _i->first==cmp;
}



/// Tests if the value for this option is empty or not.
///
/// @return True if there is no value set, else false.
bool GetOpts::Iterator::val_empty() const
{
   return _i->second.empty();
}



/// Iterates to next option in list of options.
void GetOpts::Iterator::operator++()
{
   ++_i;
}



/// Tests if this option iterator is not equal to the given option iterator.
///
/// @return True if they are not equal, else fail.
bool GetOpts::Iterator::operator!=(const Iterator& cmp)
{
   return _i!=cmp._i;
}



/// Tests if this option iterator is equal to the given option iterator.
///
/// @return True if they are equal, else fail.
bool GetOpts::Iterator::operator==(const Iterator& cmp)
{
   return _i==cmp._i;
}



/// Sets internal list iterator to iterator given.
///
/// @param i Internal iterator used to store pointer to option in list of
/// iterators.
GetOpts::Iterator::Iterator(const oplist::iterator& i):
   _i(i)
{}



}
