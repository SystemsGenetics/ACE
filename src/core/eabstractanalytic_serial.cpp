#include "eabstractanalytic_serial.h"
#include "edebug.h"



//






/*!
 * Constructs a new serial object with the given analytic as its parent. 
 *
 * @param parent The parent analytic object for this new serial object. 
 */
EAbstractAnalytic::Serial::Serial(EAbstractAnalytic* parent):
   QObject(parent)
{
   EDEBUG_FUNC(this,parent)
}
