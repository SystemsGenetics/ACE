#include "nvmemory.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
namespace AccelCompEng
{



NVMemory::NVMemory(const std::string& fileName)
{
   open(fileName);
}



NVMemory::~NVMemory()
{
   if (is_open())
   {
      close();
   }
}



void NVMemory::open(const std::string& fileName)
{
   static const char* f = __PRETTY_FUNCTION__;
   constexpr static int flags = O_CREAT|O_RDWR|O_LARGEFILE;
   constexpr static mode_t modes = S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH;
   assert<AlreadyOpen>(_fd==-1,f,__LINE__);
   _fd = ::open(fileName.c_str(),flags,modes);
   assert<SystemError>(_fd!=-1,f,__LINE__);
   bool cond;
   if (lseek64(_fd,0,SEEK_END)==0)
   {
      cond = ::write(_fd,_idStr,_idLen)==_idLen&&
             ::write(_fd,&_next,sizeof(int64_t))==sizeof(int64_t);
      assert<SystemError>(cond,f,__LINE__);
   }
   else
   {
      cond = lseek64(_fd,0,SEEK_END)>=(_idLen + sizeof(int64_t));
      assert<InvalidFile>(cond,f,__LINE__);
      char buf[_idLen];
      cond = ::lseek64(_fd,0,SEEK_SET)==0;
      assert<SystemError>(cond,f,__LINE__);
      cond = ::read(_fd,buf,_idLen)!=-1;
      assert<SystemError>(cond,f,__LINE__);
      cond = true;
      for (int i=0;i<_idLen;++i)
      {
         if (buf[i]!=_idStr[i])
         {
            cond = false;
         }
      }
      assert<InvalidFile>(cond,f,__LINE__);
      cond = ::read(_fd,&_next,sizeof(int64_t))==sizeof(int64_t);
      assert<SystemError>(cond,f,__LINE__);
      _capacity = lseek64(_fd,0,SEEK_END) - _idLen - sizeof(int64_t);
      _avail = _capacity - _next;
   }
}



bool NVMemory::is_open() const
{
   return _fd!=fnullptr;
}



void NVMemory::close()
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<AlreadyClosed>(_fd!=-1,f,__LINE__);
   ::close(_fd);
   _fd = fnullptr;
   _avail = _capacity = _next = 0;
}



void NVMemory::clear()
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NotOpen>(is_open(),f,__LINE__);
   _avail = _capacity;
   _next = 0;
}



bool NVMemory::reserve(int64_t numBytes)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NotOpen>(is_open(),f,__LINE__);
   assert<InvalidInput>(numBytes>0,f,__LINE__);
   bool ret = false;
   if (posix_fallocate64(_fd,lseek64(_fd,0,SEEK_END),numBytes)==0)
   {
      ret = true;
      _capacity += numBytes;
      _avail += numBytes;
   }
   return ret;
}



int64_t NVMemory::allocate(int64_t numBytes)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NotOpen>(is_open(),f,__LINE__);
   assert<InvalidInput>(numBytes>0,f,__LINE__);
   int64_t ret {fnullptr};
   if (numBytes<=_avail)
   {
      ret = _next;
      _next += numBytes;
      _avail -= numBytes;
      bool cond = lseek64(_fd,_idLen,SEEK_SET)==_idLen;
      assert<SystemError>(cond,f,__LINE__);
      cond = ::write(_fd,&_next,sizeof(int64_t))==sizeof(int64_t);
      assert<SystemError>(cond,f,__LINE__);
   }
   return ret;
}



int64_t NVMemory::size() const
{
   return _next;
}



int64_t NVMemory::capacity() const
{
   return _capacity;
}



int64_t NVMemory::available() const
{
   return _avail;
}



bool NVMemory::is_network_endian()
{
   return !(reinterpret_cast<const int8_t*>(&_endianTest)[0]);
}



void NVMemory::read(void* data, int64_t ptr, int64_t byteSize) const
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NotOpen>(is_open(),f,__LINE__);
   assert<InvalidInput>(ptr>=0&&byteSize>0,f,__LINE__);
   assert<FileSegFault>((ptr + byteSize)<=_next,f,__LINE__);
   int64_t seekr = ptr + _idLen + sizeof(int64_t);
   bool cond = lseek64(_fd,seekr,SEEK_SET)==seekr;
   assert<SystemError>(cond,f,__LINE__);
   cond = ::read(_fd,data,byteSize)==byteSize;
   assert<SystemError>(cond,f,__LINE__);
}



void NVMemory::write(const void* data, int64_t ptr, int64_t byteSize)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NotOpen>(is_open(),f,__LINE__);
   assert<InvalidInput>(ptr>=0&&byteSize>0,f,__LINE__);
   assert<FileSegFault>((ptr + byteSize)<=_next,f,__LINE__);
   int64_t seekr = ptr + _idLen + sizeof(int64_t);
   bool cond = lseek64(_fd,seekr,SEEK_SET)==seekr;
   assert<SystemError>(cond,f,__LINE__);
   cond = ::write(_fd,data,byteSize)==byteSize;
   assert<SystemError>(cond,f,__LINE__);
}



NVMemory::Node::Node(const std::shared_ptr<NVMemory>& mem):
   _mem(mem)
{}



NVMemory::Node::Node(const std::shared_ptr<NVMemory>& mem, int64_t ptr):
   _mem(mem),
   _ptr(ptr)
{}



NVMemory::Node::Node(const Node& copy):
   _mem(copy._mem),
   _ptr(copy._ptr),
   _size(copy._size)
{
   if (_size>0)
   {
      _data.reset(new char[_size]);
      memcpy(_data.get(),copy._mem.get(),_size);
   }
}



NVMemory::Node& NVMemory::Node::operator=(const Node& copy)
{
   _mem = copy._mem;
   _ptr = copy._ptr;
   _size = copy._size;
   if (_size>0)
   {
      _data.reset(new char[_size]);
      memcpy(_data.get(),copy._mem.get(),_size);
   }
}



std::shared_ptr<NVMemory> NVMemory::Node::mem() const
{
   return _mem;
}



void NVMemory::Node::mem(const std::shared_ptr<NVMemory>& mem)
{
   _mem = mem;
}



int64_t NVMemory::Node::addr() const
{
   return _ptr;
}



void NVMemory::Node::addr(int64_t ptr)
{
   _ptr = ptr;
}



void NVMemory::Node::allocate(int64_t numNodes)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<InvalidInput>(numNodes>0,f,__LINE__);
   assert<NullMemory>(_mem.get(),f,__LINE__);
   assert<NullData>(_size>0,f,__LINE__);
   int64_t realSize = _size*numNodes;
   if (_mem->available()<realSize)
   {
      bool cond {_mem->reserve(_mem->available()-realSize)};
      assert<AllotFail>(cond,f,__LINE__);
   }
   _ptr = _mem->allocate(realSize);
   assert<AllotFail>(_ptr!=fnullptr,f,__LINE__);
}



void NVMemory::Node::read(int64_t i)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<InvalidInput>(i>=0,f,__LINE__);
   assert<NullMemory>(_mem.get(),f,__LINE__);
   assert<NullData>(_size>0,f,__LINE__);
   assert<NullPtr>(_ptr!=fnullptr,f,__LINE__);
   _mem->read(_data.get(),_ptr+i*_size,_size);
   if (!NVMemory::is_network_endian())
   {
      flip_endian();
   }
}



void NVMemory::Node::write(int64_t i)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<InvalidInput>(i>=0,f,__LINE__);
   assert<NullMemory>(_mem.get(),f,__LINE__);
   assert<NullData>(_size>0,f,__LINE__);
   assert<NullPtr>(_ptr!=fnullptr,f,__LINE__);
   if (!NVMemory::is_network_endian())
   {
      flip_endian();
   }
   _mem->read(_data.get(),_ptr+i*_size,_size);
   if (!NVMemory::is_network_endian())
   {
      flip_endian();
   }
}



bool NVMemory::Node::operator==(const Node& cmp)
{
   return _mem.get()==cmp._mem.get()&&_ptr==cmp._ptr;
}



bool NVMemory::Node::operator!=(const Node& cmp)
{
   return _mem.get()!=cmp._mem.get()||_ptr!=cmp._ptr;
}



void NVMemory::Node::operator++()
{
   _ptr += _size;
}



void NVMemory::Node::resize(int64_t numBytes, bool copy)
{
   char* oldData {_data.release()};
   _data.reset(new char[numBytes]);
   if (oldData)
   {
      if (copy)
      {
         int64_t len {(_size>numBytes) ? numBytes : _size};
         memcpy(_data.get(),oldData,len);
      }
      delete oldData;
   }
   _size = numBytes;
}



}
