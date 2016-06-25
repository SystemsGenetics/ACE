#include <cstring>
#include "file.h"
namespace AccelCompEng
{



/// @brief Open KINC file object.
///
/// Open file memory object from file given. If the size of the file memory
/// object is zero then create a new KINC file from it, else attempt to load
/// KINC file from head location of file memory object.
///
/// @param fileName Location of file for memory object.
///
/// @exception InvalidFile The file being opened is not a valid KINC file.
File::File(const std::string& fileName):
   _mem(fileName),
   _ident(&_mem)
{
   if (_mem.size()==0)
   {
      create();
   }
   else
   {
      bool cond = _mem.size()>=_hdrSz;
      assert<InvalidFile>(cond,__LINE__);
      _hdr = _mem.head();
      _mem.sync(_hdr,FileSync::read);
      cond = _hdr.histHead()!=FileMem::nullPtr;
      assert<InvalidFile>(cond,__LINE__);
      cond = true;
      for (int i=0;i<_idSz;++i)
      {
         if (_hdr.idString()[i]!=_idString[i])
         {
            cond = false;
         }
      }
      assert<InvalidFile>(cond,__LINE__);
      _hist = hptr(new History(_mem,_hdr.histHead()));
      _ident.addr(_hdr.ident());
      _new = false;
   }
}



/// @brief Clear this object.
///
/// Clear all data in this object, including all data plugin memory, making this
/// object a new KINC file with no information.
void File::clear()
{
   _hist.reset();
   _new = true;
   _hdr = FileMem::nullPtr;
   _ident.addr(FileMem::nullPtr);
   _mem.clear();
   create();
}



/// Tests to see if this object created a new KINC file when constructed.
///
/// @return True if this is a new file, else false.
bool File::is_new()
{
   return _new;
}



/// Get reference of history object for this object.
///
/// @return History object.
History& File::history()
{
   return *_hist;
}



/// Get data plugin ident value for this object.
///
/// @return data plugin ident.
File::string File::ident() const
{
   return *_ident;
}



/// Set value for data plugin ident.
///
/// @param id Value for ident.
///
/// @exception AlreadySet The ident value for data plugin has already been set.
void File::ident(const string& id)
{
   try
   {
      _ident = id;
   }
   catch (FString::AlreadySet)
   {
      throw AlreadySet(__LINE__);
   }
   _hdr.ident() = _ident.addr();
   _mem.sync(_hdr,FileSync::write);
}



/// @brief Get data plugin memory head.
///
/// Get file memory location for the beginning or head of data plugin memory for
/// this object.
///
/// @return Location to beginning of data plugin memory.
FileMem::Ptr File::head() const
{
   return _hdr.dataHead();
}



/// Set value of file memory location for beginning of data plugin memory.
///
/// @param ptr Location of data plugin memory.
void File::head(FileMem::Ptr ptr)
{
   _hdr.dataHead() = ptr;
   _mem.sync(_hdr,FileSync::write);
}



/// Get pointer of file memory object for this object.
///
/// @return File memory object.
FileMem& File::mem()
{
   return _mem;
}



/// @brief Create new KINC file.
///
/// Create a new KINC file for this object, writing to this object's file memory
/// object.
void File::create()
{
   _mem.allot(_hdr);
   _hist = hptr(new History(_mem));
   _hdr.histHead() = _hist->addr();
   _hdr.dataHead() = FileMem::nullPtr;
   _hdr.ident() = FileMem::nullPtr;
   memcpy(_hdr.idString(),_idString,_idSz);
   _mem.sync(_hdr,FileSync::write);
}



}
