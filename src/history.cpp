#include "history.h"
#include "fstring.h"
#include <time.h>
namespace AccelCompEng
{



History::History():
   Node(sizeof(Header))
{
   init_data<Header>();
}



History::History(const std::shared_ptr<NVMemory>& mem):
   Node(sizeof(Header),mem)
{
   init_data<Header>();
}



History::History(const std::shared_ptr<NVMemory>& mem, int64_t ptr):
   Node(sizeof(Header),mem,ptr)
{
   init_data<Header>();
   load();
}



void History::load(int64_t ptr)
{
   if (addr()!=fnullptr)
   {
      _children.clear();
   }
   addr(ptr);
   load();
}



void History::init(const std::string& fileName, const std::string& object,
                   const std::string& command)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<AlreadySet>(addr()==fnullptr,f,__LINE__);
   time_t t;
   get<Header>()._timeStamp = time(&t);
   _fileName = fileName;
   _object = object;
   _command = command;
}



void History::add_child(const History& child)
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<AlreadySet>(addr()==fnullptr,f,__LINE__);
   std::unique_ptr<History> nchild {new History(mem())};
   nchild->part_copy(child);
   nchild->copy_children(child);
   _children.push_back(std::move(nchild));
}



int64_t History::write()
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<AlreadySet>(addr()==fnullptr,f,__LINE__);
   assert<NullMemory>(!is_null_memory(),f,__LINE__);
   try
   {
      int64_t ret = init_write();
      final_write();
      return ret;
   }
   catch (...)
   {
      _fileName.clear();
      _object.clear();
      _command.clear();
      _children.clear();
      addr(fnullptr);
      throw;
   }
}



const std::string& History::file_name() const
{
   return _fileName;
}



const std::string& History::object() const
{
   return _object;
}



const std::string& History::command() const
{
   return _command;
}



uint64_t History::time_stamp() const
{
   return get<Header>()._timeStamp;
}



bool History::has_children() const
{
   return _children.size()>0;
}



History::Iterator History::begin() const
{
   return Iterator(this,0);
}



History::Iterator History::end() const
{
   return Iterator(this,_children.size());
}



void History::load()
{
   try
   {
      read();
      FString fstr(mem(),addr()+sizeof(Header));
      fstr.static_buffer(_strSize);
      _fileName = fstr.str();
      fstr.bump();
      _object = fstr.str();
      fstr.bump();
      _command = fstr.str();
      int64_t ptr {get<Header>()._childHead};
      while (ptr!=fnullptr)
      {
         std::unique_ptr<History> child {new History(mem(),ptr)};
         ptr = child->get<Header>()._next;
         _children.push_back(std::move(child));
      }
   }
   catch (...)
   {
      _fileName.clear();
      _object.clear();
      _command.clear();
      _children.clear();
      addr(fnullptr);
      throw;
   }
}



void History::copy_children(const History& src)
{
   for (auto i = src._children.begin();i!=src._children.end();++i)
   {
      const std::unique_ptr<History>& child {*i};
      std::unique_ptr<History> nchild {new History(mem())};
      nchild->part_copy(*(child.get()));
      nchild->copy_children(*(child.get()));
      _children.push_back(std::move(nchild));
   }
}



void History::part_copy(const History& pCopy)
{
   _fileName = pCopy._fileName;
   _object = pCopy._object;
   _command = pCopy._command;
   get<Header>()._timeStamp = pCopy.get<Header>()._timeStamp;
}



int64_t History::init_write()
{
   allocate();
   FString fstr(mem());
   fstr.static_buffer(_strSize);
   fstr.write(_fileName);
   fstr.reset();
   fstr.write(_object);
   fstr.reset();
   fstr.write(_command);
   History* last {nullptr};
   for (auto i = _children.begin();i!=_children.end();++i)
   {
      std::unique_ptr<History>& ptr {*i};
      if (!last)
      {
         get<Header>()._childHead = ptr->init_write();
      }
      else
      {
         last->get<Header>()._next = ptr->init_write();
      }
      last = ptr.get();
   }
   return addr();
}



void History::final_write()
{
   Node::write();
   for (auto i = _children.begin();i!=_children.end();++i)
   {
      std::unique_ptr<History>& ptr {*i};
      ptr->final_write();
   }
}



void History::null_data()
{
   get<Header>()._timeStamp = 0;
   get<Header>()._next = fnullptr;
   get<Header>()._childHead = fnullptr;
}



void History::flip_endian()
{
   flip(0,8);
   flip(8,8);
   flip(16,8);
}



History::Iterator History::Iterator::begin() const
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<OutOfRange>(_i<(_p->_children.size()),f,__LINE__);
   return _p->_children[_i]->begin();
}



History::Iterator History::Iterator::end() const
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<OutOfRange>(_i<(_p->_children.size()),f,__LINE__);
   return _p->_children[_i]->end();
}



const History& History::Iterator::history() const
{
   static const char* f = __PRETTY_FUNCTION__;
   assert<OutOfRange>(_i<(_p->_children.size()),f,__LINE__);
   return *(_p->_children[_i].get());
}



const History& History::Iterator::operator*() const
{
   return *(_p->_children[_i].get());
}



const History* History::Iterator::operator->() const
{
   return _p->_children[_i].get();
}



bool History::Iterator::operator==(const Iterator& cmp) const
{
   return _p==cmp._p&&_i==cmp._i;
}



bool History::Iterator::operator!=(const Iterator& cmp) const
{
   return _p!=cmp._p||_i!=cmp._i;
}



void History::Iterator::operator++()
{
   if (_i<(_p->_children.size()))
   {
      ++_i;
   }
}



History::Iterator::Iterator(const History* p, int i):
   _p(p),
   _i(i)
{}



}
