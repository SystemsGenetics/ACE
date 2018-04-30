#include "ace_analytic_serialrun.h"
#include <memory>
#include <QTimer>
#include "ace_analytic_abstractinput.h"
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
SerialRun::SerialRun(EAbstractAnalytic::Serial* serial, AbstractInput* base, QObject* parent):
   AbstractRun(parent),
   _serial(serial),
   _base(base)
{}






/*!
 *
 * @param block  
 */
void SerialRun::addWork(std::unique_ptr<EAbstractAnalytic::Block>&& block)
{
   unique_ptr<EAbstractAnalytic::Block> result {_serial->execute(block.get())};
   block.reset();
   _base->saveResult(std::move(result));
   if ( _base->isFinished() )
   {
      emit finished();
   }
}
