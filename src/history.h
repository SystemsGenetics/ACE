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
   struct AlreadySet : public Exception { using Exception::Exception; };
   struct NullMemory : public Exception { using Exception::Exception; };
   class Iterator;
   History();
   History(const std::shared_ptr<NVMemory>& mem);
   History(const std::shared_ptr<NVMemory>& mem, int64_t ptr);
   using Node::mem;
   void load(int64_t ptr);
   void init(const std::string& fileName, const std::string& object, const std::string& command,
             uint64_t timeStamp);
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
   void copy_children(History* dest, const History& src);
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
   Iterator begin();
   Iterator end();
   const History& operator*();
   const History* operator->();
   bool operator!=(const Iterator&);
   void operator++();
private:
   Iterator(const History* p, int i);
   const History* _p;
   int _i;
};



}
#endif
