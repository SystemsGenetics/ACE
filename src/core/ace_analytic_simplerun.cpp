#include "ace_analytic_simplerun.h"
#include <QTimer>
#include "eabstractanalytic.h"



using namespace Ace::Analytic;
//






/*!
 *
 * @param analytic  
 *
 * @param parent  
 */
SimpleRun::SimpleRun(EAbstractAnalytic* analytic, QObject* parent):
   QObject(parent),
   _analytic(analytic)
{}






/*!
 */
void SimpleRun::start()
{
   QTimer::singleShot(0,this,&SimpleRun::process);
}






/*!
 */
void SimpleRun::process()
{
   if ( _next < _analytic->size() )
   {
      _analytic->process(_next,nullptr);
      next();
      QTimer::singleShot(0,this,&SimpleRun::process);
   }
   else
   {
      emit finished();
   }
}






/*!
 */
void SimpleRun::next()
{
   int percentComplete {100*_next++/_analytic->size()};
   if ( _percentComplete != percentComplete )
   {
      _percentComplete = percentComplete;
      emit progressed(_percentComplete);
   }
}
