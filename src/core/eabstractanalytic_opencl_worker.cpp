#include "eabstractanalytic_opencl_worker.h"
#include "ace_settings.h"
#include "edebug.h"



//






/*!
 * Returns the total number of threads ACE uses to run OpenCL workers in parallel. 
 *
 * @return Total number of OpenCL worker threads. 
 */
int EAbstractAnalytic::OpenCL::Worker::threadSize()
{
   EDEBUG_FUNC()

   return Ace::Settings::instance().threadSize();
}
