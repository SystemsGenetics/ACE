#ifndef ACCELCOMPENG_NVMEMORY_H
#define ACCELCOMPENG_NVMEMORY_H
#define fnullptr -1
#include <cstdint>
#include <string>
#include "exception.h"
#include <memory>
namespace AccelCompEng
{



class NVMemory
{
public:
   struct SystemError : public Exception { using Exception::Exception; };
   struct InvalidFile : public Exception { using Exception::Exception; };
   struct AlreadyOpen : public Exception { using Exception::Exception; };
   struct AlreadyClosed : public Exception { using Exception::Exception; };
   struct NotOpen : public Exception { using Exception::Exception; };
   struct FileSegFault : public Exception { using Exception::Exception; };
   struct InvalidInput : public Exception { using Exception::Exception; };
   class Node;
   NVMemory() = default;
   NVMemory(const std::string& fileName);
   ~NVMemory();
   NVMemory(const NVMemory&) = delete;
   NVMemory& operator=(const NVMemory&) = delete;
   NVMemory(NVMemory&&) = delete;
   NVMemory& operator=(NVMemory&&) = delete;
   void open(const std::string& fileName);
   bool is_open() const;
   void close();
   void clear();
   bool reserve(int64_t numBytes);
   int64_t allocate(int64_t numBytes);
   int64_t size() const;
   int64_t capacity() const;
   int64_t available() const;
   static bool is_network_endian();
private:
   void read(void* data, int64_t ptr, int64_t byteSize) const;
   void write(const void* data, int64_t ptr, int64_t byteSize);
   int _fd {fnullptr};
   int64_t _capacity {0};
   int64_t _avail {0};
   int64_t _next {0};
   static const uint32_t _endianTest {1};
   constexpr static const char* _idStr {"\33\102\104\101\124"};
   constexpr static int _idLen {5};
};



class NVMemory::Node
{
public:
   struct NullMemory : public Exception { using Exception::Exception; };
   struct NullData : public Exception { using Exception::Exception; };
   struct NullPtr : public Exception { using Exception::Exception; };
   struct AllotFail : public Exception { using Exception::Exception; };
   struct InvalidInput : public Exception { using Exception::Exception; };
   Node();
   Node(const std::shared_ptr<NVMemory>& mem);
   Node(const std::shared_ptr<NVMemory>& mem, int64_t ptr);
   Node(const Node&);
   Node& operator=(const Node&);
   Node(Node&&) = default;
   Node& operator=(Node&&) = default;
   std::shared_ptr<NVMemory> mem() const;
   void mem(const std::shared_ptr<NVMemory>& mem);
   int64_t addr() const;
   void addr(int64_t ptr);
   void allocate(int64_t numNodes = 1);
   void read(int64_t i = 0);
   void write(int64_t i = 0);
   bool operator==(const Node&);
   bool operator!=(const Node&);
   void operator++();
protected:
   template<class T> T* get_ptr();
   void resize(int64_t numBytes, bool copy = false);
   virtual void flip_endian() = 0;
private:
   std::shared_ptr<NVMemory> _mem {nullptr};
   int64_t _ptr {fnullptr};
   int64_t _size {0};
   std::unique_ptr<char> _data {nullptr};
};



template<class T> T* NVMemory::Node::get_ptr()
{
   return reinterpret_cast<T*>(_data.get());
}



}
#endif
