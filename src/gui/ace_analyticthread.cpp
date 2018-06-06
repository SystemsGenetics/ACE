#include "ace_analyticthread.h"
#include <../core/ace_analytic_abstractmanager.h>
#include <../core/eexception.h>



using namespace Ace;
//






/*!
 *
 * @param manager  
 */
AnalyticThread::AnalyticThread(Analytic::AbstractManager* manager):
   _manager(manager)
{}






/*!
 */
void AnalyticThread::check()
{
   if ( _exception )
   {
      EException e(*_exception);
      delete _exception;
      _exception = nullptr;
      throw e;
   }
}






/*!
 *
 * @param exception  
 */
void AnalyticThread::setException(const EException& exception)
{
   delete _exception;
   _exception = new EException(exception);
   _manager->deleteLater();
   exit(-1);
}






/*!
 */
void AnalyticThread::run()
{
   QThread::run();
}
