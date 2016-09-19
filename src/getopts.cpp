#include <regex>
#include "getopts.h"
namespace AccelCompEng
{



GetOpts::GetOpts(const std::string& raw):
   _orig(raw)
{
   static const char* f = __PRETTY_FUNCTION__;
   using regex = std::regex;
   using regtok = std::sregex_token_iterator;
   regex pat {"[\\s\\t]+"};
   regtok end {};
   for (regtok r {raw.begin(),raw.end(),pat,-1}; r != end ;++r)
   {
      const std::string& i {*r};
      if ( i.front() == '-' )
      {
         std::string val;
         std::string key;
         auto x = i.find_first_not_of('-');
         auto y = i.find('=');
         if ( y != std::string::npos )
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



const std::string& GetOpts::orig() const
{
   return _orig;
}



int GetOpts::com_size() const
{
   return _comms.size();
}



bool GetOpts::com_empty() const
{
   return _comms.empty();
}



int GetOpts::com_get(std::initializer_list<std::string> commands)
{
   int ret = 0;
   int count = 0;
   if ( !_comms.empty() )
   {
      for (auto i:commands)
      {
         count++;
         if ( i == _comms.front() )
         {
            ret = count;
            break;
         }
      }
   }
   return ret;
}



std::string& GetOpts::com_front()
{
   return _comms.front();
}



void GetOpts::com_pop()
{
   if ( !_comms.empty() )
   {
      _comms.pop_front();
   }
}



int GetOpts::size() const
{
   return _opts.size();
}



bool GetOpts::empty() const
{
   return _opts.empty();
}



bool GetOpts::has_opt(const std::string& opt, bool del)
{
   bool ret {false};
   for (auto i = _opts.begin(); i != _opts.end() ;)
   {
      if ( i->first == opt )
      {
         ret = true;
         if ( del )
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



GetOpts::Iterator GetOpts::begin()
{
   return {_opts.begin()};
}



GetOpts::Iterator GetOpts::end()
{
   return {_opts.end()};
}



GetOpts::Iterator GetOpts::erase(Iterator i)
{
   return {_opts.erase(i._i)};
}



const std::string& GetOpts::Iterator::key() const
{
   return _i->first;
}



bool GetOpts::Iterator::is_key(const std::string& cmp) const
{
   return _i->first==cmp;
}



bool GetOpts::Iterator::val_empty() const
{
   return _i->second.empty();
}



void GetOpts::Iterator::operator++()
{
   ++_i;
}



bool GetOpts::Iterator::operator!=(const Iterator& cmp)
{
   return _i!=cmp._i;
}



bool GetOpts::Iterator::operator==(const Iterator& cmp)
{
   return _i==cmp._i;
}



GetOpts::Iterator::Iterator(const std::list<std::pair<std::string,std::string>>::iterator& i):
   _i(i)
{}



}
