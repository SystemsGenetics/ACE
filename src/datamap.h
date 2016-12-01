#ifndef ACCELCOMPENG_DATAMAP_H
#define ACCELCOMPENG_DATAMAP_H
#include <map>
#include <string>
#include <memory>
#include "factory.h"
#include "getopts.h"
#include "exception.h"
namespace AccelCompEng
{



/// @brief Manages all data objects.
///
/// Manages list of all open data objects the program has loaded. Handles passing user commands to
/// individual data obejcts, along with selecting an active data object, opening and closing data
/// objects, and listing all loaded data objects.
class DataMap
{
public:
   struct InvalidUse : public Exception { using Exception::Exception; };
   struct AlreadyExists : public Exception { using Exception::Exception; };
   struct DoesNotExist : public Exception { using Exception::Exception; };
   struct NoSelect : public Exception { using Exception::Exception; };
   struct InvalidType : public Exception { using Exception::Exception; };
   class Iterator;
   /// Initializes object manager instance and makes sure there isn't already an instance of this
   /// class in existence.
   /// @param factory Plugin factory object.
   DataMap(Factory& factory);
   ~DataMap();
   DataMap(const DataMap&) = delete;
   DataMap& operator=(const DataMap&) = delete;
   DataMap(DataMap&&) = delete;
   DataMap& operator=(DataMap&&) = delete;
   /// Opens a data object file, optionally selected opened object.
   /// @param file Name of file to be opened as data object.
   /// @param type Object file's data type.
   /// @param select True if newly opened data object will be selected, else false.
   Data* open(const std::string& file, const std::string& type, bool select = false);
   /// Close an open data object, if data object is currently selected it is cleared to none.
   /// @param file File name of opened data object to close.
   /// @return True if the closed data object was selected and cleared, else false.
   bool close(const std::string& file);
   /// Select a data object ot have focus.
   /// @param file Name of loaded data object to select.
   void select(const std::string& file);
   /// Clear any selected data object to none.
   /// @return True if there was a selected data object and it was cleared, else
   /// false.
   bool unselect();
   /// Passes load command to selected data object.
   /// @param ops User command line.
   /// @param tm Program's terminal interface.
   void load(GetOpts& ops, Terminal& tm);
   /// Passes dump command to selected data object.
   /// @param ops User command line.
   /// @param tm Program's terminal interface.
   void dump(GetOpts& ops, Terminal& tm);
   /// Passes query command to selected data object.
   /// @param ops User command line.
   /// @param tm Program's terminal interface.
   void query(GetOpts& ops, Terminal& tm);
   /// Searches list of loaded data object and returns pointer to data object that matches the file
   /// name given, if any.
   /// @param file File name of data object to find.
   /// @return Pointer to data object that is found, else nullptr if not found.
   Data* find(const std::string& file);
   /// Returns pointer to data object that is currently selected, if any.
   /// @return Pointer to data object that is selected, else if no object is
   /// selected nullptr is returned.
   Data* current();
   Iterator begin();
   Iterator end();
   /// Return iterator of currently selected data object, if any.
   /// @return Iterator of currently selected object, end of list iterator if no
   /// object is selected.
   Iterator selected();
private:
   std::map<std::string,std::unique_ptr<Data>>::iterator get(const std::string&);
   static bool _lock;
   std::map<std::string,std::unique_ptr<Data>> _map;
   std::map<std::string,std::unique_ptr<Data>>::iterator _i;
   Factory& _factory;
};



/// @brief Forward only datamap iterator.
///
/// Forward only iterator for listing all data objects in the DataMap class.
class DataMap::Iterator
{
public:
   friend class DataMap;
   Iterator(const Iterator&) = default;
   Iterator(Iterator&&) = default;
   Iterator& operator=(const Iterator&) = default;
   Iterator& operator=(Iterator&&) = default;
   /// Get file name of iterator's data object.
   /// @return File name of data object.
   std::string file();
   /// Get iterator's data object type.
   /// @return Data object type.
   std::string type();
   void operator++();
   bool operator!=(const Iterator&);
   bool operator==(const Iterator&);
private:
   Iterator(std::map<std::string,std::unique_ptr<Data>>::iterator);
   std::map<std::string,std::unique_ptr<Data>>::iterator _i;
};



}
#endif
