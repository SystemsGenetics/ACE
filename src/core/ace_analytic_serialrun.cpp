#include "ace_analytic_serialrun.h"
#include <memory>
#include <QTimer>
#include "ace_analytic_iobase.h"
#include "eabstractanalytic_block.h"
#include "eabstractanalytic_serial.h"



using namespace std;
using namespace Ace::Analytic;
//






/*!
 *
 * @param serial  
 *
 * @param base  
 *
 * @param parent  
 */
SerialRun::SerialRun(EAbstractAnalytic::Serial* serial, IOBase* base, QObject* parent):
   Run(parent),
   _serial(serial),
   _base(base)
{}






/*!
 */
void SerialRun::start()
{
   QTimer::singleShot(0,this,&SerialRun::process);
}






/*!
 */
void SerialRun::process()
{
   if ( _base->hasWork() )
   {
      unique_ptr<EAbstractAnalytic::Block> work {_base->makeWork()};
      unique_ptr<EAbstractAnalytic::Block> result {_serial->execute(work.get())};
      _base->saveResult(std::move(result));
      QTimer::singleShot(0,this,&SerialRun::process);
   }
   else if ( _base->isFinished() )
   {
      emit finished();
   }
}
