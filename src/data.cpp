#include "data.h"
namespace AccelCompEng
{



const std::string Data::type() const
{
   return _type;
}



void Data::type(const std::string& type)
{
   _type = type;
}



}
