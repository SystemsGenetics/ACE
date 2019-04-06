#include "ace_analytic_serialrun.h"
#include <memory>
#include <QTimer>
#include "ace_analytic_abstractinput.h"
#include "edebug.h"
#include "eabstractanalyticblock.h"
#include "eabstractanalyticserial.h"



using namespace std;
using namespace Ace::Analytic;
//






/*!
 * Implements the interface that is called to add a work block to be processed by 
 * this abstract run. This implementation simply processes the block and saves the 
 * result immediately because it is serial. 
 *
 * @param block The work block that is processed. 
 */
void SerialRun::addWork(std::unique_ptr<EAbstractAnalyticBlock>&& block)
{
   EDEBUG_FUNC(this,block.get())

   // Process the given work block by calling this object's abstract serial execute 
   // interface, saving the returned result block. If this object's abstract input is 
   // finished then emit the finished signal. 
   unique_ptr<EAbstractAnalyticBlock> result {_serial->execute(block.get())};
   block.reset();
   _base->saveResult(std::move(result));
   if ( _base->isFinished() )
   {
      emit finished();
   }
}






/*!
 * Constructs a new serial run object with the given abstract serial object, 
 * abstract input object, and optional parent. 
 *
 * @param serial Pointer to the abstract serial object produced by the analytic 
 *               that is being ran. 
 *
 * @param base Pointer to the abstract input used to save all result blocks. 
 *
 * @param parent Optional parent for this new serial run. 
 */
SerialRun::SerialRun(EAbstractAnalyticSerial* serial, AbstractInput* base, QObject* parent):
   AbstractRun(parent),
   _serial(serial),
   _base(base)
{
   EDEBUG_FUNC(this,serial,base,parent)
}
