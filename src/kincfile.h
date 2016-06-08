#ifndef ACCELCOMPENG_KINCFILE_H
#define ACCELCOMPENG_KINCFILE_H
#include <string>
#include <memory>
#include "filemem.h"
#include "history.h"
#include "fstring.h"
namespace AccelCompEng
{



namespace KincFileData
{
   struct Header;
   constexpr FileMem::SizeT idSz = 4;
   constexpr FileMem::SizeT hdrSz = idSz+24;
   constexpr auto idString = "\113\111\116\103";
}

struct KincFileData::Header : FileMem::Static<hdrSz>
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
class KincFile
{
public:
   // *
   // * EXCEPTIONS
   // *
   ACE_EXCEPTION(AccelCompEng::KincFile,InvalidFile)
   ACE_EXCEPTION(AccelCompEng::KincFile,AlreadySet)
   // *
   // * DECLERATIONS
   // *
   using string = std::string;
   using FPtr = FileMem::Ptr;
   // *
   // * BASIC METHODS
   // *
   KincFile(const string&);
   // *
   // * COPY METHODS
   // *
   KincFile(const KincFile&) = delete;
   KincFile& operator=(const KincFile&) = delete;
   // *
   // * MOVE METHODS
   // *
   KincFile(KincFile&&) = delete;
   KincFile& operator=(KincFile&&) = delete;
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
   using Header = KincFileData::Header;
   using hptr = std::unique_ptr<History>;
   // *
   // * FUNCTIONS
   // *
   void create();
   // *
   // * CONSTANTS
   // *
   constexpr static auto _idString  = KincFileData::idString;
   constexpr static auto _idSz = KincFileData::idSz;
   constexpr static auto _hdrSz = KincFileData::hdrSz;
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
