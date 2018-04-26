#include "ace_analytic_simplerun.h"
#include <QTimer>
#include "ace_analytic_iobase.h"
#include "eabstractanalytic_block.h"



using namespace std;
using namespace Ace::Analytic;
//






/*!
 *
 * @param base  
 *
 * @param parent  
 */
SimpleRun::SimpleRun(IOBase* base, QObject* parent):
   Run(parent),
   _base(base)
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
   if ( _base->isFinished() )
   {
      emit finished();
   }
   else
   {
      _base->saveResult(nullptr);
      QTimer::singleShot(0,this,&SimpleRun::process);
   }
}
