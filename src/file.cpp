#include "file.h"
#include "fstring.h"
namespace AccelCompEng
{



void File::load(const std::string& fileName)
{
   static const char* f = __PRETTY_FUNCTION__;
   if ( !_header.is_null_memory() )
   {
      reset();
   }
   _header.init_mem(new NVMemory(fileName));
   try
   {
      if ( _header.rmem().size() == 0 )
      {
         _header.allocate();
         _header.write();
      }
      else
      {
         assert<InvalidFile>(_header.rmem().size()>=sizeof(Data::Header),f,__LINE__);
         _header.addr(fheadptr);
         _header.read();
         bool cond {true};
         for (int i = 0; i < Data::_idSize ;++i)
         {
            if ( _header.data()._id[i] != Data::_idString[i] )
            {
               cond = false;
               break;
            }
         }
         assert<InvalidFile>(cond,f,__LINE__);
         if ( _header.data()._identPtr != fnullptr )
         {
            FString fstr(_header.mem(),_header.data()._identPtr);
            _ident = fstr.str();
         }
         if ( _header.data()._historyHead != fnullptr )
         {
            _history.reset(new History(_header.mem(),_header.data()._historyHead));
         }
         _new = false;
      }
   }
   catch (...)
   {
      _header.init_mem(nullptr);
      _header.addr(fnullptr);
      reset();
      throw;
   }
}



void File::clear()
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullMemory>(!_header.is_null_memory(),f,__LINE__);
   assert<AlreadyNew>(!is_new(),f,__LINE__);
   _header.rmem().clear();
   reset();
   _header.allocate();
   _header.write();
   _new = true;
}



bool File::is_new()
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullMemory>(!_header.is_null_memory(),f,__LINE__);
   return _new;
}



void File::init_history()
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullMemory>(!_header.is_null_memory(),f,__LINE__);
   assert<AlreadySet>(!_history.get(),f,__LINE__);
   _history.reset(new History(_header.mem()));
}



History& File::history()
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullMemory>(!_header.is_null_memory(),f,__LINE__);
   assert<NullHistory>(_history.get(),f,__LINE__);
   return *(_history.get());
}



void File::write_history()
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullMemory>(!_header.is_null_memory(),f,__LINE__);
   assert<NullHistory>(_history.get(),f,__LINE__);
   assert<AlreadySet>(_header.data()._historyHead==fnullptr,f,__LINE__);
   _header.data()._historyHead = _history->write();
   _header.write();
}



const std::string& File::ident() const
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullMemory>(!_header.is_null_memory(),f,__LINE__);
   assert<NullIdent>(_header.data()._identPtr!=fnullptr,f,__LINE__);
   return _ident;
}



void File::ident(const std::string& ident)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullMemory>(!_header.is_null_memory(),f,__LINE__);
   assert<AlreadySet>(_header.data()._identPtr==fnullptr,f,__LINE__);
   try
   {
      FString fstr(_header.mem());
      _header.data()._identPtr = fstr.write(ident);
      _header.write();
      _ident = ident;
   }
   catch (...)
   {
      _header.data()._identPtr = fnullptr;
      throw;
   }
}



int64_t File::head() const
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullMemory>(!_header.is_null_memory(),f,__LINE__);
   return _header.data()._dataHead;
}



void File::head(int64_t ptr)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullMemory>(!_header.is_null_memory(),f,__LINE__);
   _header.data()._dataHead = ptr;
   _header.write();
}



const std::shared_ptr<NVMemory>& File::mem()
{
   return _header.mem();
}



NVMemory& File::rmem()
{
   return _header.rmem();
}



File::Data::Data():
   Node(sizeof(Header))
{
   init_data<Header>();
}



File::Data::Data(const std::shared_ptr<NVMemory>& mem):
   Node(sizeof(Header),mem)
{
   init_data<Header>();
}



File::Data::Data(const std::shared_ptr<NVMemory>& mem, int64_t ptr):
   Node(sizeof(Header),mem,ptr)
{
   init_data<Header>();
}



File::Data::Header& File::Data::data()
{
   return get<Header>();
}



const File::Data::Header& File::Data::data() const
{
   return get<Header>();
}



void File::Data::null_data()
{
   get<Header>()._historyHead = fnullptr;
   get<Header>()._dataHead = fnullptr;
   get<Header>()._identPtr = fnullptr;
}



void File::Data::flip_endian()
{
   flip(4,8);
   flip(12,8);
   flip(20,8);
}



void File::reset()
{
   _ident.clear();
   _history.reset();
   _header.null_data();
}



}
