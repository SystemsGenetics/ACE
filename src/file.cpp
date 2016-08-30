#include "file.h"
#include "fstring.h"
namespace AccelCompEng
{



const std::string& File::ident() const
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullMemory>(is_null_memory(),f,__LINE__);
   assert<NullIdent>(get<Header>()._identPtr!=fnullptr,f,__LINE__);
   return _ident;
}



void File::ident(const std::string& ident)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullMemory>(is_null_memory(),f,__LINE__);
   assert<NullIdent>(get<Header>()._identPtr==fnullptr,f,__LINE__);
   try
   {
      FString fstr(Node::mem());
      get<Header>()._identPtr = fstr.write(ident);
      write();
      _ident = ident;
   }
   catch (...)
   {
      get<Header>()._identPtr = fnullptr;
      throw;
   }
}



int64_t File::head() const
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullMemory>(is_null_memory(),f,__LINE__);
   return get<Header>()._dataHead;
}



void File::head(int64_t ptr)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullMemory>(is_null_memory(),f,__LINE__);
   get<Header>()._dataHead = ptr;
   write();
}



NVMemory& File::mem()
{
   return rmem();
}



void File::load(const std::string& fileName)
{
   static const char* f = __PRETTY_FUNCTION__;
   Node::mem(new NVMemory(fileName));
   try
   {
      if (rmem().size()==0)
      {
         allocate();
         write();
      }
      else
      {
         addr(fheadptr);
         read();
         bool cond {true};
         for (int i=0;i<_idSize;++i)
         {
            if (get<Header>()._id[i]!=_idString[i])
            {
               cond = false;
               break;
            }
         }
         assert<InvalidFile>(cond,f,__LINE__);
         if (get<Header>()._identPtr!=fnullptr)
         {
            FString fstr(Node::mem(),get<Header>()._identPtr);
            _ident = fstr.str();
         }
         if (get<Header>()._historyHead!=fnullptr)
         {
            _history.reset(new History(Node::mem(),get<Header>()._historyHead));
         }
         _new = false;
      }
   }
   catch (...)
   {
      Node::mem(nullptr);
      addr(fnullptr);
      throw;
   }
}



void File::clear()
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullMemory>(is_null_memory(),f,__LINE__);
   assert<AlreadyNew>(!is_new(),f,__LINE__);
   rmem().clear();
   _ident.clear();
   null_data();
   allocate();
   write();
   _new = true;
}



bool File::is_new()
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullMemory>(is_null_memory(),f,__LINE__);
   return _new;
}



void File::init_history()
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullMemory>(is_null_memory(),f,__LINE__);
   _history.reset(new History(Node::mem()));
}



void File::write_history()
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullMemory>(is_null_memory(),f,__LINE__);
   assert<NullHistory>(_history.get(),f,__LINE__);
   get<Header>()._historyHead = _history->write();
   write();
}



History& File::history()
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<NullMemory>(is_null_memory(),f,__LINE__);
   assert<NullHistory>(_history.get(),f,__LINE__);
   return *(_history.get());
}



void File::null_data()
{
   get<Header>()._historyHead = fnullptr;
   get<Header>()._dataHead = fnullptr;
   get<Header>()._identPtr = fnullptr;
}



void File::flip_endian()
{
   flip(4,8);
   flip(12,8);
   flip(20,8);
}



}
