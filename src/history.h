#ifndef ACCELCOMPENG_HISTORY_H
#define ACCELCOMPENG_HISTORY_H
#include <string>
#include <vector>
#include "exception.h"
#include "nvmemory.h"
namespace AccelCompEng
{



/// @brief History node.
///
/// Object that contains the history of a data object. It is recursive and contains child histories
/// of previous input files going all the way back to the beginning.
class History : private NVMemory::Node
{
public:
   struct AlreadySet : public Exception { using Exception::Exception; };
   struct NullMemory : public Exception { using Exception::Exception; };
   class Iterator;
   /// Create empty history object.
   History();
   /// Create empty history object with attached nvmemory.
   /// @param mem File memory object.
   History(const std::shared_ptr<NVMemory>& mem);
   /// Create history object with given nvmemory, loading history from given address.
   /// @param mem File memory object.
   /// @param ptr File address of history data location.
   History(const std::shared_ptr<NVMemory>& mem, int64_t ptr);
   History(const History&) = default;
   History(History&&) = default;
   History& operator=(const History&) = default;
   History& operator=(History&&) = default;
   using Node::mem;
   /// Load new history data at the given address.
   void load(int64_t ptr);
   /// Initialize a new history, this object must be empty to do this.
   /// @param fileName File name of data object.
   /// @param object The object which created the data object.
   /// @param command The user command which created the data object.
   void init(const std::string& fileName, const std::string& object, const std::string& command);
   /// Makes a new child by making a new copy of the given history.
   void add_child(const History& child);
   /// Write the new history data to file memory. This object must be initialized with new history
   /// data to do this.
   int64_t write();
   /// Get file name of data object.
   const std::string& file_name() const;
   /// Get object that created data object.
   const std::string& object() const;
   /// Get user command that created data object.
   const std::string& command() const;
   /// Get linux time stamp when this history was created.
   uint64_t time_stamp() const;
   /// Does this history have any children?
   bool has_children() const;
   Iterator begin() const;
   Iterator end() const;
private:
   struct __attribute__ ((__packed__)) Header
   {
      uint64_t _timeStamp {0};
      int64_t _next {fnullptr};
      int64_t _childHead {fnullptr};
   };
   void load();
   void copy_children(const History& src);
   void part_copy(const History& copy);
   int64_t init_write();
   void final_write();
   void null_data() override final;
   void flip_endian() override final;
   constexpr static int _strSize {16384};
   std::string _fileName;
   std::string _object;
   std::string _command;
   std::vector<std::unique_ptr<History>> _children;
};



class History::Iterator
{
public:
   struct OutOfRange : public Exception { using Exception::Exception; };
   friend class History;
   Iterator(const Iterator&) = default;
   Iterator(Iterator&&) = default;
   Iterator& operator=(const Iterator&) = default;
   Iterator& operator=(Iterator&&) = default;
   Iterator begin() const;
   Iterator end() const;
   /// Get reference to history object of this iterator.
   const History& history() const;
   const History& operator*() const;
   const History* operator->() const;
   bool operator==(const Iterator&) const;
   bool operator!=(const Iterator&) const;
   void operator++();
private:
   Iterator(const History* p, int i);
   const History* _p;
   int _i;
};



}
#endif
