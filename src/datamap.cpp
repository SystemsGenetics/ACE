#include "datamap.h"
namespace AccelCompEng
{



bool DataMap::_lock {false};



DataMap::DataMap(Factory& factory):
   _factory(factory),
   _i {_map.end()}
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<InvalidUse>(!_lock,f,__LINE__);
   _lock = true;
}



DataMap::~DataMap()
{
   _lock = false;
}



Data* DataMap::open(const std::string& file, const std::string& type, bool select)
{
   static const char* f = __PRETTY_FUNCTION__;
   using uptr = std::unique_ptr<Data>;
   bool cond = _map.find(file)==_map.end();
   assert<AlreadyExists>(cond,f,__LINE__);
   uptr nd(_factory.build_data(type));
   assert<InvalidType>(bool(nd),f,__LINE__);
   nd->type(type);
   nd->File::load(file);
   nd->init();
   auto x = _map.emplace(file,std::move(nd));
   auto i = x.first;
   if ( select )
   {
      _i = i;
   }
   return i->second.get();
}



bool DataMap::close(const std::string& file)
{
   bool ret = false;
   auto i = get(file);
   if ( _i == i )
   {
      ret = true;
      _i = _map.end();
   }
   _map.erase(i);
   return ret;
}



void DataMap::select(const std::string& file)
{
   _i = get(file);
}



bool DataMap::unselect()
{
   bool ret = false;
   if ( _i != _map.end() )
   {
      ret = true;
      _i = _map.end();
   }
   return ret;
}



void DataMap::load(GetOpts& ops, Terminal& tm)
{
   static const char* f = __PRETTY_FUNCTION__;
   bool cond = _i!=_map.end();
   assert<NoSelect>(cond,f,__LINE__);
   try
   {
      _i->second->load(ops,tm);
   }
   catch (...)
   {
      _map.erase(_i);
      _i = _map.end();
      throw;
   }
}



void DataMap::dump(GetOpts& ops, Terminal& tm)
{
   static const char* f = __PRETTY_FUNCTION__;
   bool cond = _i!=_map.end();
   assert<NoSelect>(cond,f,__LINE__);
   try
   {
      _i->second->dump(ops,tm);
   }
   catch (...)
   {
      _map.erase(_i);
      _i = _map.end();
      throw;
   }
}



void DataMap::query(GetOpts& ops, Terminal& tm)
{
   static const char* f = __PRETTY_FUNCTION__;
   bool cond = _i!=_map.end();
   assert<NoSelect>(cond,f,__LINE__);
   try
   {
      _i->second->query(ops,tm);
   }
   catch (...)
   {
      _map.erase(_i);
      _i = _map.end();
      throw;
   }
}



Data* DataMap::find(const std::string& file)
{
   try
   {
      return get(file)->second.get();
   }
   catch (DoesNotExist)
   {
      return nullptr;
   }
}



Data* DataMap::current()
{
   static const char* f = __PRETTY_FUNCTION__;
   bool cond = _i!=_map.end();
   assert<NoSelect>(cond,f,__LINE__);
   return _i->second.get();
}



DataMap::Iterator DataMap::begin()
{
   return _map.begin();
}



DataMap::Iterator DataMap::end()
{
   return _map.end();
}



DataMap::Iterator DataMap::selected()
{
   return _i;
}



/// @brief Find specific data object.
///
/// Searches list of loaded data object and returns iterator of data object that
/// matches the file name given.
///
/// @param file File name of data object to find.
/// @return Iterator to data object that is found.
inline std::map<std::string,std::unique_ptr<Data>>::iterator DataMap::get(const std::string& file)
{
   static const char* f = __PRETTY_FUNCTION__;
   auto i = _map.find(file);
   bool cond = i!=_map.end();
   assert<DoesNotExist>(cond,f,__LINE__);
   return i;
}



std::string DataMap::Iterator::file()
{
   return _i->first;
}



std::string DataMap::Iterator::type()
{
   return _i->second->type();
}



void DataMap::Iterator::operator++()
{
   ++_i;
}



bool DataMap::Iterator::operator!=(const Iterator& cmp)
{
   return _i!=cmp._i;
}



bool DataMap::Iterator::operator==(const Iterator& cmp)
{
   return _i==cmp._i;
}



/// @brief Create new datamap iterator.
///
/// Initializes data object iterator from internal container iterator.
///
/// @param i Internal container iterator.
DataMap::Iterator::Iterator(std::map<std::string,std::unique_ptr<Data>>::iterator i):
   _i(i)
{}



}
