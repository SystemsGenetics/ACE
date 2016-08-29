#ifndef ACCELCOMPENG_FSTRING_H
#define ACCELCOMPENG_FSTRING_H
#include "nvmemory.h"
namespace AccelCompEng
{



class FString : private NVMemory::Node
{
public:
   struct InvalidPtr : public Exception { using Exception::Exception; };
   struct AlreadySet : public Exception { using Exception::Exception; };
   struct NullStr : public Exception { using Exception::Exception; };
   struct NullMemory : public Exception { using Exception::Exception; };
   struct StringTooBig : public Exception { using Exception::Exception; };
   struct InvalidInput : public Exception { using Exception::Exception; };
   FString();
   FString(const std::shared_ptr<NVMemory>& mem);
   FString(const std::shared_ptr<NVMemory>& mem, int64_t ptr);
   using Node::mem;
   void load(int64_t ptr);
   void reset();
   const std::string& str() const;
   int64_t write(const std::string& str);
   void static_buffer(int size);
   void clear_buffer();
   void bump();
private:
   struct __attribute__ ((__packed__)) Header
   {
      int16_t _size {0};
      uint8_t _strip {0};
   };
   struct CString;
   void load();
   void null_data() override final;
   void flip_endian() override final;
   constexpr static uint8_t _strip {170};
   std::string _str;
   std::unique_ptr<char> _buffer {nullptr};
   int _bSize {0};
};



struct FString::CString : public NVMemory::Node
{
   using Node::Node;
   using Node::init_mem;
   using Node::give_mem;
   using Node::get;
private:
   void null_data() override final { get<char>() = '\0'; }
   void flip_endian() override final {}
};



}
#endif
