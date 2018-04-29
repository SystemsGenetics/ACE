#include "eabstractanalytic_opencl.h"
#include "ace_settings.h"
//






/*!
 *
 * @param parent  
 */
EAbstractAnalytic::OpenCL::OpenCL(EAbstractAnalytic* parent):
   QObject(parent)
{}






/*!
 * Returns the total number of threads ACE uses to run OpenCL workers in parallel. 
 *
 * @return Total number of OpenCL worker threads. 
 */
int EAbstractAnalytic::OpenCL::threadSize() const
{
   return Ace::Settings::instance().threadSize();
}
