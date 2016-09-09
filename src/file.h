#ifndef ACCELCOMPENG_File_H
#define ACCELCOMPENG_File_H
#include <string>
#include <memory>
#include "nvmemory.h"
#include "history.h"
#include "exception.h"
namespace AccelCompEng
{



class File
{
public:
   struct InvalidFile : public Exception { using Exception::Exception; };
   struct AlreadyNew : public Exception { using Exception::Exception; };
   struct NullMemory : public Exception { using Exception::Exception; };
   struct NullIdent : public Exception { using Exception::Exception; };
   struct AlreadySet : public Exception { using Exception::Exception; };
   struct NullHistory : public Exception { using Exception::Exception; };
   File() = default;
   File(const File&) = delete;
   File(File&&) = delete;
   File& operator=(const File&) = delete;
   File& operator=(File&&) = delete;
   void load(const std::string& fileName);
   void clear();
   bool is_new();
   void init_history();
   History& history();
   void write_history();
protected:
   const std::string& ident() const;
   void ident(const std::string& ident);
   int64_t head() const;
   void head(int64_t ptr);
   const std::shared_ptr<NVMemory>& mem();
   NVMemory& rmem();
private:
   struct Data : public NVMemory::Node
   {
      Data();
      Data(const std::shared_ptr<NVMemory>& mem);
      Data(const std::shared_ptr<NVMemory>& mem, int64_t ptr);
      constexpr static int _idSize {4};
      constexpr static const char* _idString = "\113\111\116\103";
      struct __attribute__ ((__packed__)) Header
      {
         char _id[_idSize] {_idString[0],_idString[1],_idString[2],_idString[3]};
         int64_t _historyHead {fnullptr};
         int64_t _dataHead {fnullptr};
         int64_t _identPtr {fnullptr};
      };
      Header& data();
      const Header& data() const;
      void null_data() final override;
      void flip_endian() final override;
   };
   void reset();
   Data _header;
   std::string _ident;
   bool _new {true};
   std::unique_ptr<History> _history {nullptr};
};



}
#endif
