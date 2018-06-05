#include "ace_analyticthread.h"
#include <../core/ace_analytic_abstractmanager.h>
#include <../core/eexception.h>



using namespace Ace;
//






/*!
 *
 * @param manager  
 *
 * @param parent  
 */
AnalyticThread::AnalyticThread(std::unique_ptr<Analytic::AbstractManager>&& manager, QObject* parent):
   QThread(parent),
   _manager(manager.get())
{
   _manager->moveToThread(this);
   manager.release()->setParent(this);
   connect(_manager,&Analytic::AbstractManager::finished
           ,this
           ,&AnalyticThread::managerFinished
           ,Qt::QueuedConnection);
}






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
 */
void AnalyticThread::run()
{
   try
   {
      _manager->initialize();
      exec();
   }
   catch (EException e)
   {
      _exception = new EException(e);
   }
}






/*!
 */
void AnalyticThread::managerFinished()
{
   exit(0);
}
