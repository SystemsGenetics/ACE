#ifndef ACCELCOMPENG_FSTRING_H
#define ACCELCOMPENG_FSTRING_H
#include "nvmemory.h"
#include <vector>
namespace AccelCompEng
{



/// @brief File memory string.
///
/// This object can read and write string data to and from a nvmemory object.
class FString : private NVMemory::Node
{
public:
   struct InvalidPtr : public Exception { using Exception::Exception; };
   struct AlreadySet : public Exception { using Exception::Exception; };
   struct NullStr : public Exception { using Exception::Exception; };
   struct NullMemory : public Exception { using Exception::Exception; };
   struct BufferOverrun : public Exception { using Exception::Exception; };
   struct InvalidInput : public Exception { using Exception::Exception; };
   /// Creates an empty object not associated to any nvmemory or address.
   FString();
   /// Creates a new object with no string data associated with the given nvmemory object.
   /// @param mem The file memory object associated with object.
   FString(const std::shared_ptr<NVMemory>& mem);
   /// Creates a new object with string data from specified address and nvmemory file object.
   /// @param mem The file memory object.
   /// @param ptr The file address where the string is located.
   FString(const std::shared_ptr<NVMemory>& mem, int64_t ptr);
   FString(const FString&) = default;
   FString(FString&&) = default;
   FString& operator=(const FString&) = default;
   FString& operator=(FString&&) = default;
   using Node::mem;
   using Node::pmem;
   /// Loads a new string from file memory from the address given.
   /// @param ptr File address of string to load.
   void load(int64_t ptr);
   /// This clears any loaded string and resets the file pointer to null.
   void reset();
   /// Get the string data that has been loaded from file memory.
   /// @return String data.
   const std::string& str() const;
   /// Writes a new string to file memory, returning the address of the new file string.
   /// @param str String data to write.
   /// @return File address where the new file string is located.
   int64_t write(const std::string& str);
   /// Allocate a static string buffer in memory for increased speed in constantly loading or
   /// writing a multiple of strings to or from file memory. The con is that you cannot write a
   /// string that exceeds the size of the bufer.
   /// @param size The size of the buffer in bytes.
   void static_buffer(int size);
   /// This clears any static buffer set, which causes the object to dynamically allocate a string
   /// buffer for each read or write to or from file memory.
   void clear_buffer();
   /// This assumes there is a file string directly proceeding the current file string loaded,
   /// bumping to that address and loading the next file string.
   void bump();
   /// Grabs list of file strings from memory, assuming that each string directly proceeds the
   /// previous one in memory. The provided fstring must already point to a valid nvmemory object.
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
