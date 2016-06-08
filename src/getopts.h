#ifndef ACCELCOMPENG_GETOPTS_H
#define ACCELCOMPENG_GETOPTS_H
#include <string>
#include <sstream>
#include <list>
#include <initializer_list>
#include "exception.h"
namespace AccelCompEng
{



/// @ingroup dataplugin
/// @brief Decomposes line of user input.
///
/// Takes a single line from the user and seperates it into commands and
/// options. A command is any word seperated by either spaces or tabs that does
/// not begin with a '-' character. An option is any word that begins with a '-'
/// character, having a key and optional value. They key and value of an option
/// are seperated using the '=' character. For example, 'one two --option=val'
/// has two commands(one and two) and one option(with a key of option and a
/// value of val).
class GetOpts
{
public:
   // *
   // * EXCEPTIONS
   // *
   ACE_EXCEPTION(AccelCompEng::GetOpts,InvalidType)
   ACE_EXCEPTION(AccelCompEng::GetOpts,InvalidSyntax)
   // *
   // * DECLERATIONS
   // *
   class Iterator;
   using string = std::string;
   using initlist = std::initializer_list<string>;
   // *
   // * BASIC METHODS
   // *
   GetOpts(const string&);
   // *
   // * FUNCTIONS
   // *
   const string& orig() const;
   int com_size() const;
   bool com_empty() const;
   int com_get(initlist);
   string& com_front();
   void com_pop();
   int size() const;
   bool empty() const;
   bool has_opt(const string&,bool = false);
   Iterator begin();
   Iterator end();
   Iterator erase(Iterator);
private:
   // *
   // * DECLERATIONS
   // *
   using clist = std::list<string>;
   using oplist = std::list<std::pair<string,string>>;
   // *
   // * VARIABLES
   // *
   /// Origional line of user input.
   string _orig;
   /// List of commands.
   clist _comms;
   /// List of options.
   oplist _opts;
};



/// Iterates through list of all options decomposed from user input.
///
/// @author Josh Burns
/// @date 24 March 2016
class GetOpts::Iterator
{
public:
   // *
   // * DECLERATIONS
   // *
   friend class GetOpts;
   using string = GetOpts::string;
   using oplist = GetOpts::oplist;
   // *
   // * FUNCTIONS
   // *
   const string& key() const;
   template<class T> T value() const;
   bool is_key(const string&) const;
   bool val_empty() const;
   // *
   // * OPERATORS
   // *
   void operator++();
   bool operator!=(const Iterator&);
   bool operator==(const Iterator&);
private:
   // *
   // * BASIC METHODS
   // *
   Iterator(const oplist::iterator&);
   // *
   // * VARIABLES
   // *
   /// Internal iterator that points to an option in GetOpts list.
   oplist::iterator _i;
};



/// Get value of this option, if any.
///
/// @param T The variable type for option's value.
/// @return The option's value.
///
/// @exception InvalidType The given type does not match the option's value or
/// there is no value set for this option.
template<class T> T GetOpts::Iterator::value() const
{
   T ret;
   using istring = std::istringstream;
   istring buffer(_i->second);
   bool cond = buffer >> ret;
   assert<InvalidType>(cond,__LINE__);
   return ret;
}



}
#endif
