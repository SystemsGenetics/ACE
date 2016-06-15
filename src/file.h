#ifndef ACCELCOMPENG_File_H
#define ACCELCOMPENG_File_H
#include <string>
#include <memory>
#include "filemem.h"
#include "history.h"
#include "fstring.h"
namespace AccelCompEng
{



namespace FileData
{
   struct Header;
   constexpr FileMem::SizeT idSz = 4;
   constexpr FileMem::SizeT hdrSz = idSz+24;
   constexpr auto idString = "\113\111\116\103";
}

struct FileData::Header : FileMem::Static<hdrSz>
{
   using FPtr = FileMem::Ptr;
   using FSizeT = FileMem::SizeT;
   using Static<hdrSz>::Static;
   char* idString() { &get<char>(0); }
   FPtr& histHead() { get<FPtr>(idSz); }
   FPtr& dataHead() { get<FPtr>(idSz+8); }
   const FPtr& dataHead() const { get<FPtr>(idSz+8); }
   FPtr& ident() { get<FPtr>(idSz+16); }
};



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
   ACE_EXCEPTION(AccelCompEng::File,InvalidFile)
   ACE_EXCEPTION(AccelCompEng::File,AlreadySet)
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
   FileMem* mem();//NOT TESTED.
private:
   // *
   // * DECLERATIONS
   // *
   using Header = FileData::Header;
   using hptr = std::unique_ptr<History>;
   // *
   // * FUNCTIONS
   // *
   void create();
   // *
   // * CONSTANTS
   // *
   constexpr static auto _idString  = FileData::idString;
   constexpr static auto _idSz = FileData::idSz;
   constexpr static auto _hdrSz = FileData::hdrSz;
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
