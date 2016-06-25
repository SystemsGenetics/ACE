#ifndef ACCELCOMPENG_DATAPLUGIN_H
#define ACCELCOMPENG_DATAPLUGIN_H
#include <string>
#include "data.h"
#include "file.h"
#include "exception.h"
#define ACE_DATA_HEADER() \
using FPtr = ::AccelCompEng::FileMem::Ptr;\
using GetOpts = ::AccelCompEng::GetOpts;\
using Terminal = ::AccelCompEng::Terminal;\
using FileMem = ::AccelCompEng::FileMem;\
using FSync = ::AccelCompEng::FileSync;\
using File = ::AccelCompEng::File;\
using FString = ::AccelCompEng::FString;
namespace AccelCompEng
{



/// @ingroup dataplugin
/// @brief Base class for data plugins.
///
/// Base class that all data plugin implementations must inherit. It brings it
/// the interface Data class that defines the virtual functions data plugins
/// must implement along with the KincFile helper class for manipulating the
/// associated binary file.
class DataPlugin : public Data, public File
{
public:
   // *
   // * DECLERATIONS
   // *
   using string = std::string;
   // *
   // * COPY METHODS
   // *
   DataPlugin(const DataPlugin&) = delete;
   DataPlugin& operator=(const DataPlugin&) = delete;
   // *
   // * MOVE METHODS
   // *
   DataPlugin(DataPlugin&&) = delete;
   DataPlugin& operator=(DataPlugin&&) = delete;
   // *
   // * FUNCTIONS
   // *
   string type();
protected:
   // *
   // * CONSTANTS
   // *
   constexpr static FileMem::Ptr fNullPtr {AccelCompEng::FileMem::nullPtr};
   // *
   // * BASIC METHODS
   // *
   DataPlugin(const string&,const string&);
private:
   // *
   // * VARIABLES
   // *
   string _type;
};



/// Get data type of this data plugin instance.
inline DataPlugin::string DataPlugin::type()
{
   return _type;
}


/// Initializes base class of data plugin instance.
///
/// @param type Data type of plugin.
/// @param fileName Name of file where data object exists.
inline DataPlugin::DataPlugin(const string& type, const string& fileName):
   _type(type),
   File(fileName)
{}



}
#endif
