#ifndef ACCELCOMPENG_DATAMAP_H
#define ACCELCOMPENG_DATAMAP_H
#include <map>
#include <string>
#include <memory>
#include "plugins.h"
#include "getopts.h"
#include "exception.h"
namespace AccelCompEng
{



/// @brief Manages all data objects.
///
/// Manages list of all open data objects the program has loaded. Handles
/// passing user commands to individual data obejcts, along with selecting an
/// active data object, opening and closing data objects, and listing all loaded
/// data objects.
///
/// @warning There can only be one instance of this class in the program.
///
/// @author Josh Burns
/// @date 21 March 2016
class DataMap
{
public:
   // *
   // * EXCEPTIONS
   // *
   ACE_EXCEPTION(AccelCompEng::DataMap,InvalidUse)
   ACE_EXCEPTION(AccelCompEng::DataMap,AlreadyExists)
   ACE_EXCEPTION(AccelCompEng::DataMap,DoesNotExist)
   ACE_EXCEPTION(AccelCompEng::DataMap,NoSelect)
   ACE_EXCEPTION(AccelCompEng::DataMap,InvalidType)
   // *
   // * DECLERATIONS
   // *
   class Iterator;
   using string = std::string;
   // *
   // * BASIC METHODS
   // *
   DataMap();
   ~DataMap();
   // *
   // * COPY METHODS
   // *
   DataMap(const DataMap&) = delete;
   DataMap& operator=(const DataMap&) = delete;
   // *
   // * MOVE METHODS
   // *
   DataMap(DataMap&&) = delete;
   DataMap& operator=(DataMap&&) = delete;
   // *
   // * FUNCTIONS
   // *
   DataPlugin* open(const string&,const string&,bool = false);
   bool close(const string&);
   void select(const string&);
   bool unselect();
   void load(GetOpts&,Terminal&);
   void dump(GetOpts&,Terminal&);
   void query(GetOpts&,Terminal&);
   DataPlugin* find(const string&);
   DataPlugin* current();
   Iterator begin();
   Iterator end();
   Iterator selected();
private:
   // *
   // * DECLERATIONS
   // *
   using Map = std::map<std::string,std::unique_ptr<DataPlugin>>;
   // *
   // * FUNCTIONS
   // *
   Map::iterator get(const string&);
   // *
   // * STATIC VARIABLES
   // *
   /// Used to determine if an instance of this class exists or not.
   static bool _lock;
   // *
   // * VARIABLES
   // *
   /// Contains all loaded data objects.
   Map _map;
   /// Iterator that points to selected data object, or end of list iterator if
   /// no object is selected.
   Map::iterator _i;
};



/// Forward only iterator for listing all data objects in the DataMap class.
///
/// @author Josh Burns
/// @date 21 March 2016
class DataMap::Iterator
{
public:
   // *
   // * DECLERATIONS
   // *
   friend class DataMap;
   using string = DataMap::string;
   // *
   // * FUNCTIONS
   // *
   string file();
   string type();
   // *
   // * OPERATORS
   // *
   void operator++();
   bool operator!=(const Iterator&);
   bool operator==(const Iterator&);
private:
   // *
   // * DECLERATIONS
   // *
   using Map = DataMap::Map;
   // *
   // * BASIC METHODS
   // *
   Iterator(Map::iterator);
   // *
   // * VARIABLES
   // *
   /// Points to current data object this iterator points to or end of list.
   Map::iterator _i;
};



}
#endif
