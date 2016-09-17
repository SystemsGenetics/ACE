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



/// Primary function called to step into the ACE platform.
///
/// @param header The header that will be printed on the command prompt.
/// @param factory The plugin object factory which ACE will use to find data and analytic plugins.
/// @param argc Argument from main.
/// @param argv Argument from main.
/// @param callBack Pointer to callback function that will be called after initialization if given.
/// @return Exit status once ACE quits, 0 on success and failure for anything else.
int run(const char* header, Factory& factory, int argc, char* argv[],
        void (*callBack)(Console&) = nullptr);



}
#endif
