#ifndef ACCELCOMPENG_HISTITEM_H
#define ACCELCOMPENG_HISTITEM_H
#include <string>
#include "filemem.h"
#include "fstring.h"
#include "exception.h"
namespace AccelCompEng
{



namespace HistItemData
{
   struct Skim;
   struct Item;
   constexpr FileMem::SizeT skimSz = 16;
   constexpr FileMem::SizeT nodeSz = 48;
}

struct HistItemData::Skim : FileMem::Static<skimSz>
{
   using FPtr = FileMem::Ptr;
   using Static<skimSz>::Static;
   FPtr& childHead() { get<FPtr>(0); }
   const FPtr& childHead() const { get<FPtr>(0); }
   FPtr& next() { get<FPtr>(8); }
};

struct HistItemData::Item : FileMem::Static<nodeSz>
{
   using FPtr = FileMem::Ptr;
   using Static<nodeSz>::Static;
   FPtr& childHead() { get<FPtr>(0); }
   const FPtr& childHead() const { get<FPtr>(0); }
   FPtr& next() { get<FPtr>(8); }
   const FPtr& next() const { get<FPtr>(8); }
   int64_t& timeStamp() { get<int64_t>(16); }
   const int64_t& timeStamp() const { get<int64_t>(16); }
   FPtr& fileNamePtr() { get<FPtr>(24); }
   FPtr& objectPtr() { get<FPtr>(32); }
   FPtr& commandPtr() { get<FPtr>(40); }
};



/// @brief History item in file memory.
///
/// Represents a single history item in file memory. Has ability to create a new
/// history item or recursively make a copy of a given history item, adding
/// additional history items of all of the copied items children. History items
/// hold four values representing the history of a single item. The first is a
/// time stamp that represents the time this item was created. The second is a
/// file name that respresents the file name this file was given. The third is
/// called the object which represents what analytic type created this object,
/// if any. The fourth is called the command which represents the full command
/// line used to create the item. Each item can also have a child item,
/// representing the beginning of a list of children which are all the input
/// items that made this item, if any. Each item also has a next pointer, which
/// is used to make forward only lists of items if they are children of another
/// item. Because this is a file memory object, all of these values except the
/// time stamp can only be set once after which point they are read only.
///
/// @author Josh Burns
/// @date 24 March 2016
class HistItem
{
public:
   // *
   // * EXCEPTIONS
   // *
   ACE_EXCEPTION(AccelCompEng::HistItem,AlreadySet)
   ACE_EXCEPTION(AccelCompEng::HistItem,IsAllocated)
   ACE_EXCEPTION(AccelCompEng::HistItem,IsNullPtr)
   ACE_EXCEPTION(AccelCompEng::HistItem,InvalidItem)
   // *
   // * DECLERATIONS
   // *
   using string = std::string;
   using FPtr = FileMem::Ptr;
   using TimeT = int64_t;
   // *
   // * BASIC METHODS
   // *
   HistItem(FileMem&,FPtr = FileMem::nullPtr);
   HistItem(FileMem*,FPtr = FileMem::nullPtr);
   // *
   // * COPY METHODS
   // *
   HistItem(const HistItem&) = delete;
   HistItem& operator=(const HistItem&) = delete;
   // *
   // * MOVE METHODS
   // *
   HistItem(HistItem&&);
   HistItem& operator=(HistItem&&);
   // *
   // * FUNCTIONS
   // *
   void allocate();
   void copy_from(const HistItem&);
   void sync();
   void timeStamp(TimeT);
   TimeT timeStamp() const;
   void fileName(const string&);
   const string& fileName() const;
   void object(const string&);
   const string& object() const;
   void command(const string&);
   const string& command() const;
   void next(FPtr);
   FileMem::Ptr next() const;
   void childHead(FPtr);
   FPtr childHead() const;
   FPtr addr() const;
   FileMem* mem() const;
   // *
   // * OPERATORS
   // *
   void operator=(FPtr);
private:
   // *
   // * DECLERATIONS
   // *
   using Item = HistItemData::Item;
   using FSizeT = FileMem::SizeT;
   // *
   // * FUNCTIONS
   // *
   void load_item();
   FPtr rec_add_item(FileMem*,FPtr);
   // *
   // * VARIABLES
   // *
   /// File memory object where history item is located.
   FileMem* _mem;
   /// File memory chunk of history item data.
   Item _item;
   /// File name of item.
   FString _fileName;
   /// Object that created item, if any.
   FString _object;
   /// Command that created item.
   FString _command;
};



}
#endif
