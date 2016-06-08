#ifndef ACCELCOMPENG_FSTRING_H
#define ACCELCOMPENG_FSTRING_H
#include "filemem.h"
namespace AccelCompEng
{



namespace FStringData
{
   struct Header;
   struct String;
   constexpr FileMem::SizeT hdrSz = 3;
   constexpr uint8_t strip = 170;
}

struct FStringData::Header : FileMem::Static<hdrSz>
{
   using Static<hdrSz>::Static;
   uint8_t& stripe() { get<uint8_t>(0); }
   uint16_t& sSize() { get<uint16_t>(1); }
};

struct FStringData::String : FileMem::Object
{
   using FPtr = FileMem::Ptr;
   using FSizeT = FileMem::SizeT;
   using Object::operator=;
   String(FSizeT size, FPtr ptr = FileMem::nullPtr): Object(size,ptr) {}
   char* c_str() { &get<char>(0); }
};



/// @ingroup dataplugin
/// @brief File memory string.
///
/// String object that stores a string in file memory. You can set the object
/// once, which will then write the string to file memory and get a file pointer
/// where it is stored. After the string has been written, or if loading a
/// string, access to the string is read only.
class FString
{
public:
   // *
   // * EXCEPTIONS
   // *
   ACE_EXCEPTION(AccelCompEng::FString,InvalidPtr)
   ACE_EXCEPTION(AccelCompEng::FString,AlreadySet)
   // *
   // * DECLERATIONS
   // *
   using string = std::string;
   using FPtr = FileMem::Ptr;
   // *
   // * BASIC METHODS
   // *
   FString(FileMem*,FPtr = FileMem::nullPtr);
   // *
   // * COPY METHODS
   // *
   FString(const FString&) = delete;
   FString& operator=(const FString&) = delete;
   // *
   // * MOVE METHODS
   // *
   FString(FString&&);
   FString& operator=(FString&&);
   // *
   // * FUNCTIONS
   // *
   void addr(FPtr);
   FPtr addr() const;
   // *
   // * OPERATORS
   // *
   const string& operator*() const;
   const string* operator->() const;
   FString& operator=(const string&);
private:
   // *
   // * DECLERATIONS
   // *
   using Header = FStringData::Header;
   using String = FStringData::String;
   // *
   // * FUNCTIONS
   // *
   void load();
   // *
   // * VARIABLES
   // *
   /// File memory object string is associated with.
   FileMem* _mem;
   /// Header chunk of string.
   Header _hdr;
   /// Actual string data.
   string _str;
};



}
#endif
