#include "ace_analytic_abstractrun.h"
#include "edebug.h"



namespace Ace
{
namespace Analytic
{






/*!
 * Constructs a new abstract run object with an optional parent.
 *
 * @param parent Optional parent for this new abstract run.
 */
AbstractRun::AbstractRun(QObject* parent)
   :
   QObject(parent)
{
   EDEBUG_FUNC(this,parent);
}

}
}
