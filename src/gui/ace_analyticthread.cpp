#include "ace_analyticthread.h"
#include "../core/ace_analytic_abstractmanager.h"
#include "../core/eexception.h"



using namespace Ace;
//






/*!
 * Create a new analytic thread with the given analytic manager. 
 *
 * @param manager Pointer to the analytic manager that should be moved to this 
 *                thread and executed within it. 
 */
AnalyticThread::AnalyticThread(Analytic::AbstractManager* manager):
   _manager(manager)
{}






/*!
 * Checks to see if this thread has caught an exception. If this thread does have 
 * an exception then it is thrown from this method. 
 */
void AnalyticThread::check()
{
   // If this object contains a caught exception then copy it to a new exception, 
   // delete the exception this object contained, and throw the new copied exception. 
   if ( _exception )
   {
      EException e(*_exception);
      delete _exception;
      _exception = nullptr;
      throw e;
   }
}






/*!
 * Set an exception for this analytic thread that was caught in the main event 
 * loop. This should only be used by the application notify method if it catches an 
 * exception within an analytic thread. 
 *
 * @param exception Exception that was caught and is copied to this analytic 
 *                  thread. 
 */
void AnalyticThread::setException(const EException& exception)
{
   // Delete any exception this thread may already contain, then create a new 
   // exception that is a copy of the one given for this thread to contain, and then 
   // call on the analytic manager to delete itself and halt execution. 
   delete _exception;
   _exception = new EException(exception);
   _manager->deleteLater();
}






/*!
 * Implements the qt interface that is the entry point for this thread. This begins 
 * the event loop for this thread. 
 */
void AnalyticThread::run()
{
   // Connect the destroyed signal of this thread's analytic manager to end its event 
   // loop and then start this thread's event loop. 
   connect(_manager,&QObject::destroyed,this,&QThread::quit);
   QThread::run();
}
