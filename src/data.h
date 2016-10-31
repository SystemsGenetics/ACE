#ifndef ACCELCOMPENG_DATA_H
#define ACCELCOMPENG_DATA_H
#include "terminal.h"
#include "getopts.h"
#include "file.h"
namespace AccelCompEng
{


/// @brief Data plugin interface.
///
/// Abstract class that defines the interface between the ACE program and any data plugin object.
/// This class is created whenever a data object is loaded and destroyed when the data object is
/// closed on the user terminal.
class Data : public File
{
public:
   Data(const Data&) = delete;
   Data& operator=(const Data&) = delete;
   Data(Data&&) = delete;
   Data& operator=(Data&&) = delete;
   virtual ~Data() = default;
   /// Get the specific type of this data plugin object.
   /// @return Object type.
   const std::string type() const;
   /// Set the specific type of this data plugin object.
   void type(const std::string& type);
   /// Initialize the data plugin object after the file is opened and the ACE interface for the
   /// plugin has been initialized.
   virtual void init() = 0;
   /// Calls the load command on the data object called by the user from the command line.
   /// @param ops User arguments and options from load command.
   /// @param tm Terminal interface for output.
   virtual void load(GetOpts& ops, Terminal& tm) = 0;
   /// Calls the dump command on the data object called by the user from the command line.
   /// @param ops User arguments and options from dump command.
   /// @param tm Terminal interface for output.
   virtual void dump(GetOpts& ops, Terminal& tm) = 0;
   /// Calls the query command on the data object called by the user from the command line.
   /// @param ops User arguments and options from query command.
   /// @param tm Terminal interface for output.
   virtual void query(GetOpts& ops, Terminal& tm) = 0;
   /// Returns true if there is no actual data within the data object.
   /// @return Is the data object empty?
   virtual bool empty() = 0;
protected:
   /// Creates a completely uninitialized new data plugin object. The file class along with the
   /// actual data plugin begins in an uninitialized state.
   Data() = default;
private:
   std::string _type;
};



}
#endif
