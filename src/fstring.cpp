#include "fstring.h"
#include <string.h>
namespace AccelCompEng
{



FString::FString():
   Node(sizeof(Header))
{
   init_mem();
}



FString::FString(const std::shared_ptr<NVMemory>& mem):
   Node(sizeof(Header),mem)
{
   init_mem();
}



FString::FString(const std::shared_ptr<NVMemory>& mem, int64_t ptr):
   Node(sizeof(Header),mem,ptr)
{
   init_mem();
   load();
}



void FString::load(int64_t ptr)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullMemory>(!is_null_memory(),f,__LINE__);
   addr(ptr);
   load();
}



void FString::reset()
{
   addr(fnullptr);
}



const std::string& FString::str() const
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullStr>(!is_null_memory()&&addr()!=fnullptr,f,__LINE__);
   return _str;
}



void FString::write(const std::string& str)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullMemory>(!is_null_memory(),f,__LINE__);
   assert<AlreadySet>(addr()==fnullptr,f,__LINE__);
   try
   {
      int len = str.size()+1;
      _str = str;
      CString c_str(len,mem());
      if (_buffer.get())
      {
         assert<StringTooBig>(len<=_bSize,f,__LINE__);
         c_str.give_mem(_buffer.get());
      }
      {
         c_str.init_mem();
      }
      allocate();
      c_str.allocate();
      get<Header>()._size = len;
      get<Header>()._strip = _strip;
      memcpy(&c_str.get<char>(),str.c_str(),len);
      Node::write();
      c_str.write();
   }
   catch (...)
   {
      _str.clear();
      addr(fnullptr);
      throw;
   }
}



void FString::static_buffer(int size)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<InvalidInput>(size>0,f,__LINE__);
   _buffer.reset(new char[size]);
}



void FString::clear_buffer()
{
   _buffer.reset();
}



void FString::bump()
{
   addr(addr()+sizeof(Header)+get<Header>()._size);
   load();
}



void FString::load()
{
   static const char* f = __PRETTY_FUNCTION__;
   try
   {
      read();
      assert<InvalidPtr>(get<Header>()._strip==_strip,f,__LINE__);
      CString c_str(get<Header>()._size,Node::mem(),addr()+3);
      if (_buffer.get())
      {
         assert<StringTooBig>(get<Header>()._size<=_bSize,f,__LINE__);
         c_str.give_mem(_buffer.get());
      }
      {
         c_str.init_mem();
      }
      c_str.read();
      _str = &c_str.get<char>();
   }
   catch (...)
   {
      addr(fnullptr);
   }
}



void FString::null_data()
{
   get<Header>()._size = 0;
   get<Header>()._strip = 0;
}



void FString::flip_endian()
{
   flip(0,2);
}



}
