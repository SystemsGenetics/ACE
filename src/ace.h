#ifndef ACCELCOMPENG_H
#define ACCELCOMPENG_H
#include "analytic.h"
#include "analyticplugin.h"
#include "clbuffer.h"
#include "clcommandqueue.h"
#include "clcontext.h"
#include "cldevice.h"
#include "cldevlist.h"
#include "clevent.h"
#include "clkernel.h"
#include "clprogram.h"
#include "console.h"
#include "data.h"
#include "datamap.h"
#include "dataplugin.h"
#include "exception.h"
#include "filemem.h"
#include "fstring.h"
#include "getopts.h"
#include "histitem.h"
#include "history.h"
#include "kincfile.h"
#include "linuxterm.h"
#include "terminal.h"
#define ACE_BEGIN_DATA DataPlugin* new_data(const std::string& type,\
                                            const std::string& name)\
                       {
                          DataPlugin* ret = nullptr;
#define ACE_DATA_PLUGIN(N,D) if (type==std::string(#N)) ret = new D(type,name);
#define ACE_END_DATA    return ret;\
                     }
#define ACE_BEGIN_ANALYTIC
#define ACE_ANALYTIC_PLUGIN(N,D)
#define ACE_END_ANALYTIC
namespace AccelCompEng
{



int run(int,char*[]);
AnalyticPlugin* new_analytic(const std::string&);
DataPlugin* new_data(const std::string&,const std::string&);



}
#endif
