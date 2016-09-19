#include "fstring.h"
#include <string.h>
namespace AccelCompEng
{



FString::FString():
   Node(sizeof(Header))
{
   init_data<Header>();
}



FString::FString(const std::shared_ptr<NVMemory>& mem):
   Node(sizeof(Header),mem)
{
   init_data<Header>();
}



FString::FString(const std::shared_ptr<NVMemory>& mem, int64_t ptr):
   Node(sizeof(Header),mem,ptr)
{
   init_data<Header>();
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



int64_t FString::write(const std::string& str)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullMemory>(!is_null_memory(),f,__LINE__);
   assert<AlreadySet>(addr()==fnullptr,f,__LINE__);
   try
   {
      int len = str.size()+1;
      _str = str;
      CString c_str(len,mem());
      if ( _buffer.get() )
      {
         assert<BufferOverrun>(len<=_bSize,f,__LINE__);
         c_str.give_data(_buffer.get());
      }
      else
      {
         c_str.init_data<char>(len);
      }
      allocate();
      c_str.allocate();
      get<Header>()._size = len;
      get<Header>()._strip = _strip;
      memcpy(c_str.pget<char>(),str.c_str(),len);
      Node::write();
      c_str.write();
      return addr();
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
   _bSize = size;
}



void FString::clear_buffer()
{
   _bSize = 0;
   _buffer.reset();
}



void FString::bump()
{
   addr(addr()+sizeof(Header)+get<Header>()._size);
   load();
}



void FString::fetch_list(FString& fstr, int64_t startAddr, int size, std::vector<std::string>& list)
{
   fstr.load(startAddr);
   list.push_back(fstr.str());
   --size;
   while ( size > 0 )
   {
      fstr.bump();
      list.push_back(fstr.str());
      --size;
   }
}



/// @brief Load file string.
///
/// Loads a file string into objects system memory.
void FString::load()
{
   static const char* f = __PRETTY_FUNCTION__;
   try
   {
      read();
      assert<InvalidPtr>(get<Header>()._strip==_strip,f,__LINE__);
      CString c_str(get<Header>()._size,Node::mem(),addr()+3);
      if ( _buffer.get() )
      {
         assert<BufferOverrun>(get<Header>()._size<=_bSize,f,__LINE__);
         c_str.give_data(_buffer.get());
      }
      else
      {
         c_str.init_data<char>(get<Header>()._size);
      }
      c_str.read();
      _str = c_str.pget<char>();
   }
   catch (...)
   {
      addr(fnullptr);
      throw;
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
