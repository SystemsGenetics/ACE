#ifndef ACCELCOMPENG_FILEMEM_H
#define ACCELCOMPENG_FILEMEM_H
#include <string>
#include <cstring>
#include <cstdint>
#include <unistd.h>
#include <fcntl.h>
#include "exception.h"
#define ACE_FMEM_STATIC(N,S) \
struct N : public ::AccelCompEng::FileMem::Static<S>\
{\
   using ::AccelCompEng::FileMem::Static<S>::Static;
#define ACE_FMEM_OBJECT(N) \
struct N : public ::AccelCompEng::FileMem::Object\
{
#define ACE_FMEM_VAL(N,T,L) \
   T& N() { get<T>(L); }\
   const T& N() const { get<T>(L); }
#define ACE_FMEM_END() };
namespace AccelCompEng
{



/// @ingroup dataplugin
/// Defines what type of file syncing to be done.
enum class FileSync {
   read, ///< Read data from file to buffer.
   write ///< Write data from buffer to file.
};



/// @ingroup dataplugin
/// @brief Binary file interface.
///
/// Provides interface to use a given binary file as a swap like memory device
/// for reading and writing. Allows allocation of new chunks of memory within
/// the file but does not allow deallocation of given memory. Also allows for
/// basic syncing operations between the file memory and buffer objects.
/// Addresses that point to memory within the file are defined as the Ptr type.
/// Sizes relating to the file memory are defined as the SizeT type.
///
/// @author Josh Burns
/// @date 22 March 2016
class FileMem
{
public:
   // *
   // * EXCEPTIONS
   // *
   struct SystemError : public Exception { using Exception::Exception; };
   struct InvalidFile : public Exception { using Exception::Exception; };
   struct FileSegFault : public Exception { using Exception::Exception; };
   struct OutOfMemory : public Exception { using Exception::Exception; };
   // *
   // * DECLERATIONS
   // *
   using Ptr = uint64_t;
   using SizeT = uint64_t;
   template<int S> struct Static;
   struct Object;
   class Map;
   // *
   // * BASIC METHODS
   // *
   FileMem(const std::string&);
   ~FileMem();
   // *
   // * COPY METHODS
   // *
   FileMem(const FileMem&) = delete;
   FileMem& operator=(const FileMem&) = delete;
   // *
   // * MOVE METHODS
   // *
   FileMem(FileMem&&) = delete;
   FileMem& operator=(FileMem&&) = delete;
   // *
   // * FUNCTIONS
   // *
   void clear();
   bool reserve(SizeT);
   template<class T> bool expand(T&,SizeT = 1);
   SizeT size() const;
   SizeT capacity() const;
   SizeT available() const;
   template<class T> void allocate(T&,SizeT = 1);
   template<class T> void allot(T&,SizeT = 1);
   template<class T> void sync(T&,FileSync,Ptr = 0);
   Ptr head();
   // *
   // * CONSTANTS
   // *
   /// Value defined as the nullptr for file memory pointers.
   constexpr static Ptr nullPtr {0xffffffffffffffffll};
   /// Ident string at beginning of any memory object file.
   constexpr static const char* _identString {"\33\102\104\101\124"};
   /// Size of ident string.
   constexpr static int _idLen {5};
private:
   // *
   // * FUNCTIONS
   // *
   Ptr fallocate(SizeT);
   void write(const void* data,Ptr,SizeT);
   void read(void*,Ptr,SizeT) const;
   // *
   // * VARIABLES
   // *
   /// System file descriptor of open file.
   int _fd;
   /// The total size of memory in file object, both free and used.
   SizeT _capacity;
   /// Total size of available memory for allocation in file object.
   SizeT _available;
   /// Pointer to next allocation of file memory and total size of file memory
   /// already allocated.
   Ptr _next;
};



/// @brief Static buffer object.
///
/// Static buffer object that provides access to an element in file memory. The
/// buffer is stored in system memory and using the sync command it will write
/// from or to file memory. It also stores a file memory address location for
/// where the object is located in file memory.
///
/// @param S The size of the buffer object in bytes.
///
/// @author Josh Burns
/// @date 22 March 2016
template<int S> struct FileMem::Static
{
   // *
   // * DECLERATIONS
   // *
   friend class FileMem;
   // *
   // * BASIC METHODS
   // *
   Static() = default;
   Static(Ptr);
   // *
   // * FUNCTIONS
   // *
   template<class T> T& get(int);
   template<class T> const T& get(int) const;
   Ptr addr() const;
   Ptr addr(Ptr);
private:
   // *
   // * VARIABLES
   // *
   /// Byte array of object.
   char bytes[S];
   /// File memory pointer of object.
   Ptr ptr {FileMem::nullPtr};
   // *
   // * CONSTANTS
   // *
   /// Size of  the buffer object in bytes.
   constexpr static SizeT size {S};
};



/// @brief Dynamic buffer object.
///
/// Provides same functionality as FileMem::Static, however this provides
/// dynamic size of the buffer object as a constructor argument.
///
/// @author Josh Burns
/// @date 22 March 2016
struct FileMem::Object
{
   // *
   // * DECLERATIONS
   // *
   friend class FileMem;
   // *
   // * BASIC METHODS
   // *
   Object(SizeT,Ptr = nullPtr);
   ~Object();
   // *
   // * COPY METHODS
   // *
   Object(const Object&);
   Object& operator=(const Object&);
   // *
   // * MOVE METHODS
   // *
   Object(Object&&);
   Object& operator=(Object&&);
   // *
   // * FUNCTIONS
   // *
   template<class T> T& get(int);
   template<class T> const T& get(int) const;
   Ptr addr() const;
   Ptr addr(Ptr);
private:
   // *
   // * VARIABLES
   // *
   /// Byte array of object.
   char* bytes;
   /// Size of  the buffer object in bytes.
   SizeT size;
   /// File memory pointer of object.
   Ptr ptr {FileMem::nullPtr};
};




/// @brief Reserves additional file memory by object amount.
///
/// Reserves additional space in file memory to be used for allocation, based
/// off number of buffer objects.
///
/// @param T Buffer object type used as size base.
/// @param obj Buffer object to be used as size base.
/// @param amt Total number of given objects to reserve new space for.
/// @return Returns true if new space reserved, else false.
template<class T> inline bool FileMem::expand(T& obj, SizeT amt)
{
   return reserve(obj.size*amt);
}



/// Allocates new space in file memory for given number of objects.
///
/// @param T Object type to allocate.
/// @param obj Object buffer instance to allocate.
/// @param amt Total number of object buffers to allovate.
template<class T> void FileMem::allocate(T& obj, SizeT amt)
{
   if (amt>0)
   {
      obj.ptr = fallocate(obj.size*amt);
   }
}



/// Reserves additional space for allocation if needed, then allocates new space
/// in file memory for given number of objects.
///
/// @param T Object type to allocate.
/// @param obj Object buffer instance to allocate.
/// @param amt Total number of object buffers to allovate.
template<class T> void FileMem::allot(T& obj, SizeT amt)
{
   if (amt>0)
   {
      SizeT size = obj.size*amt;
      if (size>_available)
      {
         reserve(size-_available);
      }
      obj.ptr = fallocate(size);
   }
}



/// @brief Syncs buffer object with file memory.
///
/// Syncs buffer object with file memory in one of two possible ways. If
/// reading, then file memory is written to the buffer object. If writing, then
/// the buffer object is written to file memory. Optional increment within
/// file memory is also given.
///
/// @param T Object type to sync.
/// @param obj Object buffer instance to sync.
/// @param which What type of sync operation to be done.
/// @param inc Incrememnt beyond address of buffer object to use for file
/// memory.
///
/// @exception FileSegFault The file pointer of object buffer is nullptr.
template<class T> void FileMem::sync(T& obj, FileSync which, Ptr inc)
{
   static const char* f = __PRETTY_FUNCTION__;
   bool cond = obj.ptr!=nullPtr;
   assert<FileSegFault>(cond,f,__LINE__);
   Ptr seekr = obj.ptr + obj.size*inc;
   switch (which)
   {
   case FileSync::read:
      read(obj.bytes,seekr,obj.size);
      break;
   case FileSync::write:
      write(obj.bytes,seekr,obj.size);
      break;
   }
}



/// Initializes static buffer object with given pointer.
///
/// @param p File Pointer where object exists in file memory.
template<int S> FileMem::Static<S>::Static(Ptr p):
   ptr(p)
{}



/// @brief Get variable from buffer.
///
/// Get variable from byte array buffer of object with given indent into array.
///
/// @param S Variable type to get from raw byte array.
/// @param n Indent into raw byte array in bytes.
/// @return Variable within byte array with given indent.
template<int S> template<class T> T& FileMem::Static<S>::get(int n)
{
   return *reinterpret_cast<T*>(&bytes[n]);
}



/// @brief Get value from buffer.
///
/// Get read only value from byte array buffer of object with given indent into
/// array.
///
/// @param S Variable type to get from raw byte array.
/// @param n Indent into raw byte array in bytes.
/// @return Read only value within byte array with given indent.
template<int S> template<class T> const T& FileMem::Static<S>::get(int n) const
{
   return *reinterpret_cast<const T*>(&bytes[n]);
}



/// Return file pointer where object is located in file memory.
///
/// @return File pointer location.
template<int S> inline FileMem::Ptr FileMem::Static<S>::addr() const
{
   return ptr;
}



/// Sets new location for buffer object in file memory.
///
/// @param p File pointer of new location.
template<int S> FileMem::Ptr FileMem::Static<S>::addr(Ptr p)
{
   ptr = p;
   return p;
}



/// @brief Get variable from buffer.
///
/// Get variable from byte array buffer of object with given indent into array.
///
/// @param S Variable type to get from raw byte array.
/// @param n Indent into raw byte array in bytes.
/// @return Variable within byte array with given indent.
template<class T> T& FileMem::Object::get(int n)
{
   return *reinterpret_cast<T*>(&bytes[n]);
}



/// @brief Get value from buffer.
///
/// Get read only value from byte array buffer of object with given indent into
/// array.
///
/// @param S Variable type to get from raw byte array.
/// @param n Indent into raw byte array in bytes.
/// @return Read only value within byte array with given indent.
template<class T> const T& FileMem::Object::get(int n) const
{
   return *reinterpret_cast<const T*>(&bytes[n]);
}



}
#endif
