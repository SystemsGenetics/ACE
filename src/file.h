#ifndef ACCELCOMPENG_File_H
#define ACCELCOMPENG_File_H
#include <string>
#include <memory>
#include "nvmemory.h"
#include "history.h"
#include "exception.h"
namespace AccelCompEng
{


/// @brief File attached to data object.
///
/// This represents the data file attached to a data plugin object. It provides an interface both to
/// the data plugin for accessing the file along with an interface for ACE itself to prepare the
/// data object.
class File
{
public:
   struct InvalidFile : public Exception { using Exception::Exception; };
   struct AlreadyNew : public Exception { using Exception::Exception; };
   struct NullMemory : public Exception { using Exception::Exception; };
   struct NullIdent : public Exception { using Exception::Exception; };
   struct AlreadySet : public Exception { using Exception::Exception; };
   struct NullHistory : public Exception { using Exception::Exception; };
   /// Create an empty file object that is not initialized.
   File() = default;
   File(const File&) = delete;
   File(File&&) = delete;
   File& operator=(const File&) = delete;
   File& operator=(File&&) = delete;
   /// Loads a data from from the given file name. This is meant for ACE internally and should not
   /// be used by a data plugin.
   /// @param fileName File name.
   void load(const std::string& fileName);
   /// Clear the loaded data file, emptying it of all data and reseting all pointers to null. This
   /// is meant for ACE internally and should not be used by a data plugin.
   void clear();
   /// Returns true if the file that was loaded did not exist and was just created.
   /// @return Is this a new file?
   bool is_new();
   /// Creates a new history object that is associated with this file object. This is meant for ACE
   /// internally and should not be used by a data plugin.
   void init_history();
   /// Provides a reference to the history object of this file object. This is meant for ACE
   /// internally and should not be used by a data plugin.
   /// @return History reference.
   History& history();
   /// If this is a new file and history is only in system memory, this writes all that information
   /// permanently to the file object.
   void write_history();
protected:
   /// Get the ident string of the file object. The default is empty.
   /// @return Identification.
   const std::string& ident() const;
   /// Set the ident string of the file object. This can only be done once.
   void ident(const std::string& ident);
   /// Get the head file pointer of the file object. The default is a null pointer.
   /// @return Head pointer.
   int64_t head() const;
   /// Set the head file pointer of the file object.
   void head(int64_t ptr);
   /// Get the pointer to the nvmemory object used to access the memory in the file object.
   /// @return Nvmemory pointer.
   const std::shared_ptr<NVMemory>& mem();
   /// Get a reference to the nvmemory object used to access the memory in the file object.
   /// @return Nvmemory rerefence.
   NVMemory& rmem();
private:
   struct Data : public NVMemory::Node
   {
      Data();
      Data(const std::shared_ptr<NVMemory>& mem);
      Data(const std::shared_ptr<NVMemory>& mem, int64_t ptr);
      constexpr static int _idSize {4};
      constexpr static const char* _idString = "\113\111\116\103";
      struct __attribute__ ((__packed__)) Header
      {
         char _id[_idSize] {_idString[0],_idString[1],_idString[2],_idString[3]};
         int64_t _historyHead {fnullptr};
         int64_t _dataHead {fnullptr};
         int64_t _identPtr {fnullptr};
      };
      Header& data();
      const Header& data() const;
      void null_data() final override;
      void flip_endian() final override;
   };
   void reset();
   Data _header;
   std::string _ident;
   bool _new {true};
   std::unique_ptr<History> _history {nullptr};
};



}
#endif
