#ifndef ACCELCOMPENG_File_H
#define ACCELCOMPENG_File_H
#include <string>
#include <memory>
#include "filemem.h"
#include "history.h"
#include "fstring.h"
namespace AccelCompEng
{



/// @ingroup dataplugin
/// @brief Base file utility class for data plugin.
///
/// Opens and manages a file memory object for a data plugin object. Provides
/// functions for the data plugin to interface with the file memory object. Also
/// provides a history object that is stored within the same file.
class File
{
public:
   // *
   // * EXCEPTIONS
   // *
   struct InvalidFile : public Exception { using Exception::Exception; };
   struct AlreadySet : public Exception { using Exception::Exception; };
   // *
   // * DECLERATIONS
   // *
   using string = std::string;
   using FPtr = FileMem::Ptr;
   // *
   // * BASIC METHODS
   // *
   File(const string&);
   // *
   // * COPY METHODS
   // *
   File(const File&) = delete;
   File& operator=(const File&) = delete;
   // *
   // * MOVE METHODS
   // *
   File(File&&) = delete;
   File& operator=(File&&) = delete;
   // *
   // * FUNCTIONS
   // *
   void clear();
   bool is_new();
   History& history();
protected:
   // *
   // * FUNCTIONS
   // *
   string ident() const;
   void ident(const string&);
   FPtr head() const;
   void head(FPtr);
   FileMem& mem();
private:
   // *
   // * CONSTANTS
   // *
   constexpr static int _hdrSz {28};
   constexpr static int _idSz {4};
   constexpr static auto _idString = "\113\111\116\103";
   // *
   // * FILEMEM TYPES
   // *
   ACE_FMEM_STATIC(Header,_hdrSz)
      using FPtr = FileMem::Ptr;
      char* idString() { &get<char>(0); }
      ACE_FMEM_VAL(histHead,FPtr,_idSz)
      ACE_FMEM_VAL(dataHead,FPtr,_idSz+8)
      ACE_FMEM_VAL(ident,FPtr,_idSz+16)
   ACE_FMEM_END()
   // *
   // * DECLERATIONS
   // *
   //using Header = FileData::Header;
   using hptr = std::unique_ptr<History>;
   // *
   // * FUNCTIONS
   // *
   void create();
   // *
   // * VARIABLES
   // *
   /// File memory object.
   FileMem _mem;
   /// Remembers if the file opened is new or not.
   bool _new {true};
   /// Pointer to history object of file.
   hptr _hist {nullptr};
   /// Header for KINC file.
   Header _hdr;
   /// Custom data plugin ident value.
   FString _ident;
};



}
#endif
