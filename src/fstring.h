#ifndef ACCELCOMPENG_FSTRING_H
#define ACCELCOMPENG_FSTRING_H
#include "filemem.h"
namespace AccelCompEng
{



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
   // * CONSTANTS
   // *
   constexpr static FileMem::SizeT _hdrSz {3};
   constexpr static uint8_t _strip {170};
   // *
   // * FILEMEM TYPES
   // *
   ACE_FMEM_STATIC(Header,_hdrSz)
      ACE_FMEM_VAL(stripe,uint8_t,0)
      ACE_FMEM_VAL(sSize,uint16_t,1)
   ACE_FMEM_END()
   ACE_FMEM_OBJECT(String)
      using FPtr = FileMem::Ptr;
      using FSizeT = FileMem::SizeT;
      String(FSizeT size, FPtr ptr = FileMem::nullPtr):
         Object(size,ptr) {}
      using Object::operator=;
      char* c_str() { &get<char>(0); }
   ACE_FMEM_END()
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
