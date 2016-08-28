#include "nvmemory.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
namespace AccelCompEng
{



const uint32_t NVMemory::_endianTest {1};



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
   bool cond = lseek64(_fd,_idLen,SEEK_SET)==_idLen;
   assert<SystemError>(cond,f,__LINE__);
   cond = ::write(_fd,&_next,sizeof(int64_t))==sizeof(int64_t);
   assert<SystemError>(cond,f,__LINE__);
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



NVMemory::Node::Node(int size):
   _size(size)
{
   static const char* f = __PRETTY_FUNCTION__;
   if (size<=0)
   {
      _size = 0;
      assert<InvalidInput>(false,f,__LINE__);
   }
}



NVMemory::Node::Node(int size, const std::shared_ptr<NVMemory>& mem, int64_t ptr):
   _size(size),
   _mem(mem),
   _ptr(ptr)
{
   static const char* f = __PRETTY_FUNCTION__;
   if (size<=0)
   {
      _size = 0;
      assert<InvalidInput>(false,f,__LINE__);
   }
}



NVMemory::Node::Node(const Node& copy):
   _mem(copy._mem),
   _ptr(copy._ptr),
   _size(copy._size)
{
   if (copy._data.get())
   {
      _data.reset(new char[_size]);
      memcpy(_data.get(),copy._data.get(),_size);
   }
}



NVMemory::Node::Node(Node&& move):
   _mem(move._mem),
   _ptr(move._ptr),
   _size(move._size)
{
   move._mem.reset();
   move._ptr = fnullptr;
   if (move._data.get())
   {
      _data.reset(new char[_size]);
      memcpy(_data.get(),move._data.get(),_size);
      move.null_data();
   }
}



NVMemory::Node& NVMemory::Node::operator=(const Node& copy)
{
   _mem = copy._mem;
   _ptr = copy._ptr;
   _size = copy._size;
   if (copy._data.get())
   {
      _data.reset(new char[_size]);
      memcpy(_data.get(),copy._data.get(),_size);
   }
   else
   {
      _data.reset();
   }
}



NVMemory::Node& NVMemory::Node::operator=(Node&& move)
{
   _mem = move._mem;
   _ptr = move._ptr;
   _size = move._size;
   move._mem.reset();
   move._ptr = fnullptr;
   if (move._data.get())
   {
      _data.reset(new char[_size]);
      memcpy(_data.get(),move._data.release(),_size);
      move.null_data();
   }
   else
   {
      _data.reset();
   }
}



bool NVMemory::Node::is_null_memory() const
{
   return _mem.get()==nullptr;
}



std::shared_ptr<NVMemory> NVMemory::Node::mem() const
{
   return _mem;
}



NVMemory& NVMemory::Node::rmem()
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullMemory>(_mem.get(),f,__LINE__);
   return *(_mem.get());
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
   int64_t realSize = _size*numNodes;
   if (_mem->available()<realSize)
   {
      bool cond {_mem->reserve(realSize-_mem->available())};
      assert<AllotFail>(cond,f,__LINE__);
   }
   _ptr = _mem->allocate(realSize);
   assert<AllotFail>(_ptr!=fnullptr,f,__LINE__);
}



void NVMemory::Node::read(int64_t i)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<InvalidInput>(i>=0,f,__LINE__);
   assert<NullData>(_data.get(),f,__LINE__);
   assert<NullMemory>(_mem.get(),f,__LINE__);
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
   assert<NullData>(_data.get(),f,__LINE__);
   assert<NullMemory>(_mem.get(),f,__LINE__);
   assert<NullPtr>(_ptr!=fnullptr,f,__LINE__);
   if (!NVMemory::is_network_endian())
   {
      flip_endian();
   }
   _mem->write(_data.get(),_ptr+i*_size,_size);
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



void NVMemory::Node::init_mem()
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<DataExists>(!_data.get(),f,__LINE__);
   _data.reset(new char[_size]);
}



void NVMemory::Node::flip(int i, int len)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<InvalidInput>(len==2||len==4||len==8,f,__LINE__);
   assert<NullData>(_data.get(),f,__LINE__);
   assert<OutOfRange>((i+len)<=_size,f,__LINE__);
   char* sw {&(_data.get()[i])};
   char tmp;
   switch (len)
   {
   case 2:
      tmp = sw[0];
      sw[0] = sw[1];
      sw[1] = tmp;
      break;
   case 4:
      tmp = sw[0];
      sw[0] = sw[3];
      sw[3] = tmp;
      tmp = sw[1];
      sw[1] = sw[2];
      sw[2] = tmp;
      break;
   case 8:
      tmp = sw[0];
      sw[0] = sw[7];
      sw[7] = tmp;
      tmp = sw[1];
      sw[1] = sw[6];
      sw[6] = tmp;
      tmp = sw[2];
      sw[2] = sw[5];
      sw[5] = tmp;
      tmp = sw[3];
      sw[3] = sw[4];
      sw[4] = tmp;
      break;
   }
}



}
