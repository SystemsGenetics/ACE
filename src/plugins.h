#ifndef ACCELCOMPENG_PLUGINS_H
#define ACCELCOMPENG_PLUGINS_H
#include <string>
#include "analyticplugin.h"
#include "dataplugin.h"
#define ACE_BEGIN_DATA \
::AccelCompEng::DataPlugin* ::AccelCompEng::new_data( const std::string& type,\
                                                      const std::string& name)\
{\
   ::AccelCompEng::DataPlugin* ret = nullptr;
#define ACE_DATA_PLUGIN(N,D) \
   if (type==std::string(#N)) ret = new D(type,name);
#define ACE_END_DATA return ret; }
#define ACE_BEGIN_ANALYTIC \
::AccelCompEng::AnalyticPlugin* ::AccelCompEng::new_analytic(\
                                                       const std::string& type)\
{\
   ::AccelCompEng::AnalyticPlugin* ret = nullptr;
#define ACE_ANALYTIC_PLUGIN(N,A) \
   if (type==std::string(#N)) ret = new A();
#define ACE_END_ANALYTIC return ret; }
namespace AccelCompEng
{



AnalyticPlugin* new_analytic(const std::string&);
DataPlugin* new_data(const std::string&,const std::string&);



}
#endif
