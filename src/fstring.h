#ifndef ACCELCOMPENG_FSTRING_H
#define ACCELCOMPENG_FSTRING_H
#include "nvmemory.h"
#include <vector>
namespace AccelCompEng
{



class FString : private NVMemory::Node
{
public:
   struct InvalidPtr : public Exception { using Exception::Exception; };
   struct AlreadySet : public Exception { using Exception::Exception; };
   struct NullStr : public Exception { using Exception::Exception; };
   struct NullMemory : public Exception { using Exception::Exception; };
   struct BufferOverrun : public Exception { using Exception::Exception; };
   struct InvalidInput : public Exception { using Exception::Exception; };
   FString();
   FString(const std::shared_ptr<NVMemory>& mem);
   FString(const std::shared_ptr<NVMemory>& mem, int64_t ptr);
   FString(const FString&) = default;
   FString(FString&&) = default;
   FString& operator=(const FString&) = default;
   FString& operator=(FString&&) = default;
   using Node::mem;
   using Node::pmem;
   void load(int64_t ptr);
   void reset();
   const std::string& str() const;
   int64_t write(const std::string& str);
   void static_buffer(int size);
   void clear_buffer();
   void bump();
   /// @brief Generates list of strings from memory.
   ///
   /// Grabs list of file strings from memory, assuming that each string directly proceeds the
   /// previous one in memory. The provided fstring must already point to a valid nvmemory object.
   ///
   /// @param fstr Initialized and empty fstring that points to valid nvmemory object.
   /// @param startAddr The address of the first string in memory in list of strings.
   /// @param size The total number of strings that should be grabbed from memory.
   /// @param list The list of strings that will be generated from memory.
   static void fetch_list(FString& fstr, int64_t startAddr, int size,
                          std::vector<std::string>& list);
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
   using Node::init_data;
   using Node::give_data;
   using Node::pget;
private:
   void null_data() override final { get<char>() = '\0'; }
   void flip_endian() override final {}
};



}
#endif
