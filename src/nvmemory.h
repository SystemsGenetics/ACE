#ifndef ACCELCOMPENG_NVMEMORY_H
#define ACCELCOMPENG_NVMEMORY_H
/// Value used in file memory for nullptr in relation to pointers
#define fnullptr -1
/// Value used in file memory for beginning of its memory in relation to pointers
#define fheadptr 0
#include <cstdint>
#include <string>
#include "exception.h"
#include <memory>
namespace AccelCompEng
{



/// @brief Non-Volatile memory.
///
/// This represents a single object of non-volatile memory. You can use it much like system memory,
/// reading and writing to it using pointers just like memory but contextual to a single object of
/// this type.
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
   /// Create new object that is empty and has no file memory.
   NVMemory() = default;
   /// Create new object using given file for its file memory.
   /// @param fileName File to use for file memory.
   NVMemory(const std::string& fileName);
   ~NVMemory();
   NVMemory(const NVMemory&) = delete;
   NVMemory(NVMemory&&) = delete;
   NVMemory& operator=(const NVMemory&) = delete;
   NVMemory& operator=(NVMemory&&) = delete;
   /// Open given file to be used for file memory. This object cannot already have a file for its
   /// memory.
   /// @param fileName File to use for file memory.
   void open(const std::string& fileName);
   /// Does this object have an open file for its memory?
   bool is_open() const;
   /// Close the object memory file, must have a file open to close.
   void close();
   /// Clear the file memory, erasing any used space.
   void clear();
   /// Make new space in file memory that can be used for allocation.
   /// @param numBytes Total space to reserve in bytes.
   bool reserve(int64_t numBytes);
   /// Allocate new space in file memory to be used.
   /// @param numBytes Total size in bytes to allocate.
   /// @return File pointer that points to new space in file memory.
   int64_t allocate(int64_t numBytes);
   /// Get total size of used space in file memory.
   int64_t size() const;
   /// Get total size of all space in file memory.
   int64_t capacity() const;
   /// Get total size of available space for allocation in file memory.
   int64_t available() const;
   /// Is the host machine network endian?
   /// @return True if the host machine is network endian.
   static bool is_network_endian();
private:
   void read(void* data, int64_t ptr, int64_t byteSize) const;
   void write(const void* data, int64_t ptr, int64_t byteSize);
   int _fd {fnullptr};
   int64_t _capacity {0};
   int64_t _avail {0};
   int64_t _next {0};
   static const uint32_t _endianTest;
   constexpr static const char* _idStr {"\33\102\104\101\124"};
   constexpr static int _idLen {5};
};



/// @brief File memory node.
///
/// This is a kind of smart pointer into file memory that points to a specific address in a specific
/// nvmemory object. It provides the basic operations needed to interact and change the file object
/// and address, along with ways to read and write its data using file memory. You provide it with
/// The data type that the node is responsible for by initializing data, or provide it with already
/// allocated system memory, either way giving it the size of the data to work with in bytes. Any
/// class inheriting this class MUST initialize its data in its constructor.
class NVMemory::Node
{
public:
   struct NullMemory : public Exception { using Exception::Exception; };
   struct NullData : public Exception { using Exception::Exception; };
   struct NullPtr : public Exception { using Exception::Exception; };
   struct AllotFail : public Exception { using Exception::Exception; };
   struct InvalidInput : public Exception { using Exception::Exception; };
   struct DataExists : public Exception { using Exception::Exception; };
   struct OutOfRange : public Exception { using Exception::Exception; };
   struct NullSize : public Exception { using Exception::Exception; };
   struct SizeMismatch : public Exception { using Exception::Exception; };
   /// Create new node with no attached file object or address.
   /// @param size Size of data in bytes.
   Node(int size);
   /// Create new node with attached file object and possible address.
   /// @param size Size of data in bytes.
   /// @param mem File object.
   /// @param ptr Optional argument giving address, default is null pointer.
   Node(int size, const std::shared_ptr<NVMemory>& mem, int64_t ptr = fnullptr);
   Node(const Node&);
   Node(Node&&);
   Node& operator=(const Node&);
   Node& operator=(Node&&);
   /// Does this node have no attached file object?
   /// @return True if this node has no file object.
   bool is_null_memory() const;
   /// Get shared pointer of node's file object.
   /// @return Pointer to node's file object.
   const std::shared_ptr<NVMemory>& mem() const;
   /// Get reference to this node's file object.
   /// @return Reference to file object.
   NVMemory& rmem() const;
   /// Get pointer to this node's file object.
   /// @return Pointer to file object.
   NVMemory* pmem() const;
   /// Set this node's file object.
   /// @param mem File object.
   void mem(const std::shared_ptr<NVMemory>& mem);
   /// Give pointer to new file object. This node will then take full ownership of the new file
   /// object and delete its memory when this object is deleted along with any other object that
   /// shared it.
   void init_mem(NVMemory* memPtr);
   /// Get address of this node.
   /// @return Node address.
   int64_t addr() const;
   /// Set address of this node.
   /// @param ptr New address.
   void addr(int64_t ptr);
   /// Allocate new space in file memory for this node. This will overwrite any previous address
   /// for this node.
   /// @param numNodes Optional argument of how many nodes to allocate, must be 1 or greater.
   void allocate(int64_t numNodes = 1);
   /// Read data from file memory.
   /// @param Optional argument giving increment into file memory to read, cannot be negative.
   void read(int64_t i = 0);
   /// Write data to file memory.
   /// @param Optional argument giving increment into file memory to write, cannot be negative.
   void write(int64_t i = 0);
   bool operator==(const Node&);
   bool operator!=(const Node&);
   void operator++();
protected:
   /// Initialize the data for this node. New system memory will be allocated with the type and size
   /// given, and because the type is a template, if it is a class its members will be initialized
   /// too.
   /// @tparam T Data type that will be allocated in system memory.
   /// @param size Optional argument giving number of T to allocate, must be greater than 0.
   template<class T> void init_data(int size = 1);
   /// Give pointer to system memory that will be used for the data buffer. This node will not take
   /// ownership of the pointer and is not responsible for its deletion.
   /// @tparam T Data type of the memory being given.
   /// @param ptr System pointer to data.
   template<class T> void give_data(T* ptr);
   /// Get reference to node's data.
   /// @tparam T Data type.
   /// @return Data reference.
   template<class T> T& get();
   /// Get reference to node's data.
   /// @tparam T Data type.
   /// @return Data reference.
   template<class T> const T& get() const;
   /// Get pointer to node's data.
   /// @tparam T Data type.
   /// @return Data pointer.
   template<class T> T* pget();
   /// Get pointer to node's data.
   /// @tparam T Data type.
   /// @return Data pointer.
   template<class T> const T* pget() const;
   /// Reverse the endianness of bytes within this node's data.
   /// @param i Increment into data in bytes.
   /// @param len Number of bytes with will be flipped, must be 2, 4, or 8.
   void flip(int i, int len);
   /// Called by this node when it is being moved and the data contained must be made null.
   virtual void null_data() = 0;
   /// Called by this node when the endianness of its data must be reversed.
   virtual void flip_endian() = 0;
private:
   std::shared_ptr<NVMemory> _mem {nullptr};
   int64_t _ptr {fnullptr};
   int64_t _size;
   std::unique_ptr<char> _own {nullptr};
   char* _data {nullptr};
};



template<class T> void NVMemory::Node::init_data(int size)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<InvalidInput>(size>0,f,__LINE__);
   assert<DataExists>(!_data,f,__LINE__);
   assert<SizeMismatch>(sizeof(T)*size==_size,f,__LINE__);
   _own.reset(reinterpret_cast<char*>(new T[size]));
   _data = _own.get();
}



template<class T> void NVMemory::Node::give_data(T* ptr)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<DataExists>(!_data,f,__LINE__);
   _data = reinterpret_cast<char*>(ptr);
}



template<class T> T& NVMemory::Node::get()
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullData>(_data,f,__LINE__);
   return *reinterpret_cast<T*>(_data);
}



template<class T> const T& NVMemory::Node::get() const
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullData>(_data,f,__LINE__);
   return *reinterpret_cast<T*>(_data);
}



template<class T> T* NVMemory::Node::pget()
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullData>(_data,f,__LINE__);
   return reinterpret_cast<T*>(_data);
}



template<class T> const T* NVMemory::Node::pget() const
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullData>(_data,f,__LINE__);
   return reinterpret_cast<T*>(_data);
}



}
#endif
