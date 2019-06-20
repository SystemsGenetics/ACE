#include "eabstractanalyticserial.h"
#include "ace_qmpi.h"
#include "ace_settings.h"
#include "edebug.h"






/*!
 * Constructs a new serial object with the given analytic as its parent.
 *
 * @param parent The parent analytic object for this new serial object.
 */
EAbstractAnalyticSerial::EAbstractAnalyticSerial(EAbstractAnalytic* parent)
   :
   QObject(parent)
{
   EDEBUG_FUNC(this,parent);
}

