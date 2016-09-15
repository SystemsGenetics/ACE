#ifndef ACCELCOMPENG_ACE_H
#define ACCELCOMPENG_ACE_H
#include "src/analytic.h"
#include "src/clbuffer.h"
#include "src/clcommandqueue.h"
#include "src/clcontext.h"
#include "src/clevent.h"
#include "src/clkernel.h"
#include "src/clprogram.h"
#include "src/console.h"
#include "src/data.h"
#include "src/fstring.h"
#include "src/getopts.h"
#include "src/file.h"
#include "src/factory.h"
#include "src/linuxterm.h"
namespace AccelCompEng
{



int run(const char*,Factory&,int,char*[]);



}
#endif
