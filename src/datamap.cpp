#include "datamap.h"
namespace AccelCompEng
{



bool DataMap::_lock {false};



/// @brief Initialize object manager.
///
/// Initializes object manager instance and makes sure there isn't already an
/// instance of this class in existence.
///
/// @exception InvalidUse An instance of this class already exists.
DataMap::DataMap():
   _i {_map.end()}
{
   assert<InvalidUse>(!_lock,__LINE__);
   _lock = true;
}



/// Releases boolean lock so new instance can be made.
DataMap::~DataMap()
{
   _lock = false;
}



/// Opens a data object file, optionally selected opened object.
///
/// @param file Name of file to be opened as data object.
/// @param type Object file's data type.
/// @param select True if newly opened data object will be selected, else false.
///
/// @exception AlreadyExists Data object with same file name as one given is
/// already loaded in the manager.
/// @exception InvalidType Data type cannot be found within the KINCPlugins
/// system.
DataPlugin* DataMap::open(const string& file, const string& type, bool select)
{
   using uptr = std::unique_ptr<DataPlugin>;
   bool cond = _map.find(file)==_map.end();
   assert<AlreadyExists>(cond,__LINE__);
   uptr nd(new_data(type,file));
   assert<InvalidType>(bool(nd),__LINE__);
   auto x = _map.emplace(file,std::move(nd));
   auto i = x.first;
   if (select)
   {
      _i = i;
   }
   return i->second.get();
}



/// @brief Close data object.
///
/// Close an open data object, if data object is currently selected it is
/// cleared to none.
///
/// @param file File name of opened data object to close.
/// @return True if the closed data object was selected and cleared, else false.
bool DataMap::close(const string& file)
{
   bool ret = false;
   auto i = get(file);
   if (_i==i)
   {
      ret = true;
      _i = _map.end();
   }
   _map.erase(i);
   return ret;
}



/// Select a data object ot have focus.
///
/// @param file Name of loaded data object to select.
void DataMap::select(const string& file)
{
   _i = get(file);
}



/// Clear any selected data object to none.
///
/// @return True if there was a selected data object and it was cleared, else
/// false.
bool DataMap::unselect()
{
   bool ret = false;
   if (_i!=_map.end())
   {
      ret = true;
      _i = _map.end();
   }
   return ret;
}



/// Passes load command to selected data object.
///
/// @param ops User command line.
/// @param tm Program's terminal interface.
///
/// @exception NoSelect No data object is selected.
void DataMap::load(GetOpts& ops, Terminal& tm)
{
   bool cond = _i!=_map.end();
   assert<NoSelect>(cond,__LINE__);
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



/// Passes dump command to selected data object.
///
/// @param ops User command line.
/// @param tm Program's terminal interface.
///
/// @exception NoSelect No data object is selected.
void DataMap::dump(GetOpts& ops, Terminal& tm)
{
   bool cond = _i!=_map.end();
   assert<NoSelect>(cond,__LINE__);
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



/// Passes query command to selected data object.
///
/// @param ops User command line.
/// @param tm Program's terminal interface.
///
/// @exception NoSelect No data object is selected.
void DataMap::query(GetOpts& ops, Terminal& tm)
{
   bool cond = _i!=_map.end();
   assert<NoSelect>(cond,__LINE__);
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



/// @brief Find specific data object.
///
/// Searches list of loaded data object and returns pointer to data object that
/// matches the file name given, if any.
///
/// @param file File name of data object to find.
/// @return Pointer to data object that is found, else nullptr if not found.
DataPlugin* DataMap::find(const string& file)
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



/// Returns pointer to data object that is currently selected, if any.
///
/// @return Pointer to data object that is selected, else if no object is
/// selected nullptr is returned.
DataPlugin* DataMap::current()
{
   bool cond = _i!=_map.end();
   assert<NoSelect>(cond,__LINE__);
   return _i->second.get();
}



/// Get beginning of list iterator.
///
/// @return Iterator.
DataMap::Iterator DataMap::begin()
{
   return _map.begin();
}



/// Get end of list iterator.
///
/// @return Iterator.
DataMap::Iterator DataMap::end()
{
   return _map.end();
}



/// Return iterator of currently selected data object, if any.
///
/// @return Iterator of currently selected object, end of list iterator if no
/// object is selected.
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
///
/// @exception DoesNotExist Data object with given file name does not exist in
/// list of opened data objects.
inline DataMap::Map::iterator DataMap::get(const string& file)
{
   auto i = _map.find(file);
   bool cond = i!=_map.end();
   assert<DoesNotExist>(cond,__LINE__);
   return i;
}



/// Get file name of iterator's data object.
///
/// @return File name of data object.
DataMap::Iterator::string DataMap::Iterator::file()
{
   return _i->first;
}



/// Get iterator's data object type.
///
/// @return Data object type.
DataMap::Iterator::string DataMap::Iterator::type()
{
   return _i->second->type();
}



/// Iterate to next data object in list of objects.
void DataMap::Iterator::operator++()
{
   ++_i;
}



/// Compare between two data object iterators.
///
/// @return False if they do not point to the same data object, else true.
bool DataMap::Iterator::operator!=(const Iterator& cmp)
{
   return _i!=cmp._i;
}



/// Compare between two data object iterators.
///
/// @return True if they point to the same data object, else false.
bool DataMap::Iterator::operator==(const Iterator& cmp)
{
   return _i==cmp._i;
}



/// Initializes data object iterator from internal container iterator.
///
/// @param i Internal container iterator.
DataMap::Iterator::Iterator(Map::iterator i):
   _i(i)
{}



}
