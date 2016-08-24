#include "data.h"
namespace AccelCompEng
{



/// Get data type of this data plugin instance.
Data::string Data::type()
{
   return _type;
}


/// Initializes base class of data plugin instance.
///
/// @param type Data type of plugin.
/// @param fileName Name of file where data object exists.
Data::Data(const string& type, const string& fileName):
   _type(type),
   File(fileName)
{}



}
