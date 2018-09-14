#include "eabstractanalytic_input.h"
#include "edebug.h"



//






/*!
 * Constructs a new input object with the given analytic as its parent. 
 *
 * @param parent The parent analytic object for this new input object. 
 */
EAbstractAnalytic::Input::Input(EAbstractAnalytic* parent):
   QObject(parent)
{
   EDEBUG_FUNC(this,parent)
}
