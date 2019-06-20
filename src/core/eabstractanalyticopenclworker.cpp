#include "eabstractanalyticopenclworker.h"
#include "ace_settings.h"
#include "edebug.h"






/*!
 * Returns the total number of threads ACE uses to run OpenCL workers in
 * parallel.
 *
 * @return Total number of OpenCL worker threads.
 */
int EAbstractAnalyticOpenCLWorker::threadSize()
{
   EDEBUG_FUNC();
   return Ace::Settings::instance().threadSize();
}

