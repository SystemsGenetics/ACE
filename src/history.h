#ifndef ACCELCOMPENG_HISTORY_H
#define ACCELCOMPENG_HISTORY_H
#include <string>
#include <vector>
#include "exception.h"
#include "nvmemory.h"
namespace AccelCompEng
{



class History : private NVMemory::Node
{
public:
   struct AlreadySet : public Exception { using Exception::Exception; };
   struct NullMemory : public Exception { using Exception::Exception; };
   class Iterator;
   History();
   History(const std::shared_ptr<NVMemory>& mem);
   History(const std::shared_ptr<NVMemory>& mem, int64_t ptr);
   History(const History&) = default;
   History(History&&) = default;
   History& operator=(const History&) = default;
   History& operator=(History&&) = default;
   using Node::mem;
   void load(int64_t ptr);
   void init(const std::string& fileName, const std::string& object, const std::string& command);
   void add_child(const History& child);
   int64_t write();
   const std::string& file_name() const;
   const std::string& object() const;
   const std::string& command() const;
   uint64_t time_stamp() const;
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
