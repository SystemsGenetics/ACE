#ifndef ACCELCOMPENG_PLUGINS_H
#define ACCELCOMPENG_PLUGINS_H
#include <string>
#include "analyticplugin.h"
#include "dataplugin.h"
namespace AccelCompEng
{



AnalyticPlugin* new_analytic(const std::string&);
DataPlugin* new_data(const std::string&,const std::string&);



}
#endif
