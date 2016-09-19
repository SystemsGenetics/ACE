#ifndef ACCELCOMPENG_GETOPTS_H
#define ACCELCOMPENG_GETOPTS_H
#include <string>
#include <sstream>
#include <list>
#include <initializer_list>
#include "exception.h"
namespace AccelCompEng
{



/// @brief Decomposes line of user input.
///
/// Takes a single line from the user and seperates it into commands and options. A command is any
/// word seperated by either spaces or tabs that does not begin with a '-' character. An option is
/// any word that begins with a '-' character, having a key and optional value. They key and value
/// of an option are seperated using the '=' character. For example, 'one two --option=val' has two
/// commands(one and two) and one option(with a key of option and a value of val).
class GetOpts
{
public:
   struct InvalidType : public Exception { using Exception::Exception; };
   struct InvalidSyntax : public Exception { using Exception::Exception; };
   class Iterator;
   /// Decomposes the raw user input line given into commands and options, adding them to this new
   /// object's lists.
   /// @param raw User input line.
   GetOpts(const std::string& raw);
   GetOpts(const GetOpts&) = default;
   GetOpts(GetOpts&&) = default;
   GetOpts& operator=(const GetOpts&) = default;
   GetOpts& operator=(GetOpts&&) = default;
   /// Get origional user input line for this object.
   /// @return Origional user input.
   const std::string& orig() const;
   /// Get total number of commands left for this object.
   /// @return Number of commands.
   int com_size() const;
   /// Tests if the commands list for this object is empty.
   /// @return True if commands list is empty, else false.
   bool com_empty() const;
   /// Interrogate the command at front of list for this object, comparing it to initializer list of
   /// strings given seeing if it matches any of them.
   /// @param commands List of values to match front of list command by.
   /// @return 0 if no match is found in list of command values, else increment into initializer
   /// list where match was found starting with 1 for beginning of list.
   int com_get(std::initializer_list<std::string>);
   /// Returns reference to string of command in front of list for this object.
   /// @return Front of list command.
   std::string& com_front();
   /// Remove the command at front of list of commands for this object.
   void com_pop();
   /// Get total number of options left for this object.
   /// @return Number of options.
   int size() const;
   /// Tests if the options list for this object is empty.
   /// @return True if options list is empty, else false.
   bool empty() const;
   /// Interrogate list of options to see if the key of any option matches the value given,
   /// optionally removing all matches from list of options if found.
   /// @param opt Key value to match against.
   /// @param del True to remove all matching options, else false.
   /// @return True if one or more matches found, else false.
   bool has_opt(const std::string&,bool = false);
   Iterator begin();
   Iterator end();
   /// Removes option from list by iterator given.
   /// @param i Iterator that points to option that will be removed.
   /// @return Iterator that points to next option in list or one past end of list if the removed
   /// option was at back of list.
   Iterator erase(Iterator);
private:
   std::string _orig;
   std::list<std::string> _comms;
   std::list<std::pair<std::string,std::string>> _opts;
};



/// @brief Getopts iterator.
///
/// Iterates through list of all options decomposed from user input.
class GetOpts::Iterator
{
public:
   friend class GetOpts;
   /// Get key value of this option.
   /// @return Key value.
   const std::string& key() const;
   /// Get value of this option, if any.
   /// @tparam T The variable type for option's value.
   /// @return The option's value.
   template<class T> T value() const;
   /// Tests if the given key value matches the key value of this option.
   /// @return True if they match, else fail.
   bool is_key(const std::string&) const;
   /// Tests if the value for this option is empty or not.
   /// @return True if there is no value set, else false.
   bool val_empty() const;
   void operator++();
   bool operator!=(const Iterator&);
   bool operator==(const Iterator&);
private:
   Iterator(const std::list<std::pair<std::string,std::string>>::iterator&);
   std::list<std::pair<std::string,std::string>>::iterator _i;
};



template<class T> T GetOpts::Iterator::value() const
{
   static const char* f = __PRETTY_FUNCTION__;
   T ret;
   using istring = std::istringstream;
   istring buffer(_i->second);
   bool cond = buffer >> ret;
   assert<InvalidType>(cond,f,__LINE__);
   return ret;
}



}
#endif
