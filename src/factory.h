#ifndef ACCELCOMPENG_FACTORY_H
#define ACCELCOMPENG_FACTORY_H
#include <string>
#include "analytic.h"
#include "data.h"
namespace AccelCompEng
{



class Factory
{
public:
   virtual Data* build_data(const std::string& type) = 0;
   virtual Analytic* build_analytic(const std::string& type) = 0;
};



}
#endif
