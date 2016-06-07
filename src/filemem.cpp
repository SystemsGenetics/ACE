#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>
#include "filemem.h"
#include <iostream>
namespace AccelCompEng
{



/// @brief Initializes file memory.
///
/// Opens file memory object and initializing the file if empty, else reading
/// header information from it. In either case all instance variables are
/// initialized.
///
/// @param fileName Name of file where memory object is located.
///
/// @exception SystemError A system call, open, lseek64, write, or read, failed
/// to execute.
/// @exception InvalidFile The file being opened is not a valid file memory
/// object.
FileMem::FileMem(const std::string& fileName):
   _fd(-1),
   _capacity(0),
   _available(0),
   _next(0)
{
   constexpr static int flags = O_CREAT|O_RDWR|O_LARGEFILE;
   constexpr static mode_t modes = S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH;
   _fd = open(fileName.c_str(),flags,modes);
   assert<SystemError>(_fd!=-1,__LINE__);
   bool cond;
   if (lseek64(_fd,0,SEEK_END)==0)
   {
      cond = ::write(_fd,_identString,_idLen)==_idLen&&
             ::write(_fd,&_next,sizeof(Ptr))==sizeof(Ptr);
      assert<SystemError>(cond,__LINE__);
   }
   else
   {
      cond = lseek64(_fd,0,SEEK_END)>=(_idLen + sizeof(Ptr));
      assert<InvalidFile>(cond,__LINE__);
      char buf[_idLen];
      cond = ::lseek64(_fd,0,SEEK_SET)==0;
      assert<SystemError>(cond,__LINE__);
      cond = ::read(_fd,buf,_idLen)!=-1;
      assert<SystemError>(cond,__LINE__);
      cond = true;
      for (int i=0;i<_idLen;++i)
      {
         if (buf[i]!=_identString[i])
         {
            cond = false;
         }
      }
      assert<InvalidFile>(cond,__LINE__);
      cond = ::read(_fd,&_next,sizeof(Ptr))==sizeof(Ptr);
      assert<SystemError>(cond,__LINE__);
      _capacity = lseek64(_fd,0,SEEK_END) - _idLen - sizeof(Ptr);
      _available = _capacity - _next;
   }
}



/// Closes the file descriptor associated with file memory object.
FileMem::~FileMem()
{
   if (_fd!=-1)
   {
      close(_fd);
   }
}



/// @brief Clears all allocated space.
///
/// Clears all allocated space in file memory object, losing any data saved
/// to the file. Resets size to 0 and available space to total capacity.
///
/// @exception SystemError A system call, lseek64 or write, failed to execute.
void FileMem::clear()
{
   _available = _capacity;
   _next = 0;
   bool cond = lseek64(_fd,_idLen,SEEK_SET)==_idLen;
   assert<SystemError>(cond,__LINE__);
   cond = ::write(_fd,&_next,sizeof(Ptr))==sizeof(Ptr);
   assert<SystemError>(cond,__LINE__);
}



/// @brief Reserves additional file memory by raw bytes.
///
/// Reserves additional space in file memory to be used for allocation, based
/// off raw size in bytes.
///
/// @param size Additional space to reserve in bytes.
/// @return Returns true if new space reserved, else false.
bool FileMem::reserve(SizeT size)
{
   bool ret = false;
   if (posix_fallocate64(_fd,lseek64(_fd,0,SEEK_END),size)==0)
   {
      ret = true;
      _capacity += size;
      _available += size;
   }
   return ret;
}


/// Returns total size of allocated space in file memory.
///
/// @return Total allocated size in bytes.
FileMem::SizeT FileMem::size() const
{
   return _next;
}



/// @brief Returns total file memory size.
///
/// Returns total size of file memory object, being the sum of allocated and
/// unallocated space.
///
/// @return Total capacity in bytes.
FileMem::SizeT FileMem::capacity() const
{
   return _capacity;
}



/// Returns space available for allocation in file memory.
///
/// @return Available space in bytes.
FileMem::SizeT FileMem::available() const
{
   return _available;
}



/// Returns head or beginning pointer of file memory object.
///
/// @return Head of file memory.
FileMem::Ptr FileMem::head()
{
   return 0;
}



/// Allocates new chunk of memory in file object with given raw byte size.
///
/// @param size Size in bytes for new chunk of file memory.
/// @return File pointer to new chunk of file memory.
///
/// @exception OutOfMemory There is not enough available space left to allocate
/// requested size.
/// @exception SystemError A system call, lseek64 or write, failed executing.
FileMem::Ptr FileMem::fallocate(SizeT size)
{
   assert<OutOfMemory>(size<=_available,__LINE__);
   Ptr ret = _next;
   _next += size;
   _available -= size;
   bool cond = lseek64(_fd,_idLen,SEEK_SET)==_idLen;
   assert<SystemError>(cond,__LINE__);
   cond = ::write(_fd,&_next,sizeof(Ptr))==sizeof(Ptr);
   assert<SystemError>(cond,__LINE__);
   return ret;
}



/// Write raw byte array to file memory with given address.
///
/// @param data Pointer to byte array.
/// @param ptr File pointer to file memory location.
/// @param size Size of byte array in bytes.
///
/// @exception FileSegFault The location given in file memory exceeds the total
/// size of allocated space in file memory.
/// @exception SystemError A system call, lseek64 or write, failed executing.
void FileMem::write(const void* data, Ptr ptr, SizeT size)
{
   assert<FileSegFault>((ptr + size)<=_next,__LINE__);
   int64_t seekr = ptr + _idLen + sizeof(Ptr);
   bool cond = lseek64(_fd,seekr,SEEK_SET)==seekr;
   assert<SystemError>(cond,__LINE__);
   cond = ::write(_fd,data,size)==size;
   assert<SystemError>(cond,__LINE__);
}



/// Read file memory with given address to byte array.
///
/// @param data Pointer to byte array.
/// @param ptr File pointer to file memory location.
/// @param size Size of byte array in bytes.
///
/// @exception FileSegFault The location given in file memory exceeds the total
/// size of allocated space in file memory.
/// @exception SystemError A system call, lseek64 or write, failed executing.
void FileMem::read(void* data, Ptr ptr, SizeT size) const
{
   assert<FileSegFault>((ptr + size)<=_next,__LINE__);
   int64_t seekr = ptr + _idLen + sizeof(Ptr);
   bool cond = lseek64(_fd,seekr,SEEK_SET)==seekr;
   assert<SystemError>(cond,__LINE__);
   cond = ::read(_fd,data,size)==size;
   assert<SystemError>(cond,__LINE__);
}



/// Initializes dynamic object buffer with given size and file memory location.
///
/// @param s Size of buffer object in bytes.
/// @param p File pointer to location of object in file memory.
FileMem::Object::Object(SizeT s, Ptr p):
   size(s),
   ptr(p)
{
   bytes = new char[size];
}



/// Destroys byte array in heap memory.
FileMem::Object::~Object()
{
   if (bytes)
   {
      delete[] bytes;
   }
}



/// Copies given dynamic object.
///
/// @param obj Object buffer to be copied.
FileMem::Object::Object(const Object& obj):
   size(obj.size),
   ptr(obj.ptr)
{
   bytes = new char[size];
   memcpy(bytes,obj.bytes,size);
}



/// Copies given dynamic object, overwriting what this object currently stores.
///
/// @param obj Object buffer to be copied.
/// @return Reference to method's object.
FileMem::Object& FileMem::Object::operator=(const Object& obj)
{
   size = obj.size;
   if (bytes)
   {
      delete[] bytes;
   }
   bytes = new char[size];
   memcpy(bytes,obj.bytes,size);
   ptr = obj.ptr;
}



/// Moves given dynamic object.
///
/// @param tmp Object buffer to take data from.
FileMem::Object::Object(Object&& tmp):
   size(tmp.size),
   bytes(tmp.bytes),
   ptr(tmp.ptr)
{
   tmp.bytes = nullptr;
   tmp.size = 0;
   tmp.ptr = nullPtr;
}



/// Moves given dynamic object, overwriting what this object currently stores.
///
/// @param tmp Object buffer to take data from.
/// @return Reference to method's object.
FileMem::Object& FileMem::Object::operator=(Object&& tmp)
{
   size = tmp.size;
   bytes = tmp.bytes;
   ptr = tmp.ptr;
   tmp.bytes = nullptr;
   tmp.size = 0;
   tmp.ptr = nullPtr;
}



/// Return file pointer where object is located in file memory.
///
/// @return File pointer location.
FileMem::Ptr FileMem::Object::addr() const
{
   return ptr;
}



/// Sets new location for buffer object in file memory.
///
/// @param p File pointer of new location.
FileMem::Ptr FileMem::Object::addr(Ptr p)
{
   ptr = p;
   return p;
}


}
