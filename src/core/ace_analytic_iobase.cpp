#include "ace_analytic_iobase.h"
#include "eabstractanalytic_block.h"
#include "eexception.h"



using namespace std;
using namespace Ace::Analytic;
//






/*!
 */
IOBase::~IOBase()
{
   for (auto result: _hopper.values())
   {
      delete result;
   }
}






/*!
 *
 * @param result  
 */
void IOBase::saveResult(std::unique_ptr<EAbstractAnalytic::Block>&& result)
{
   if ( !result )
   {
      writeResult(nullptr);
   }
   else if ( result->index() == nextResult() )
   {
      writeResult(std::move(result));
      while ( _hopper.contains(nextResult()) )
      {
         writeResult(unique_ptr<EAbstractAnalytic::Block>(_hopper.take(nextResult())));
      }
   }
   else
   {
      _hopper.insert(result->index(),result.release());
   }
}






/*!
 *
 * @param result  
 */
void IOBase::writeResult(std::unique_ptr<EAbstractAnalytic::Block>&& result)
{
   result.reset();
   E_MAKE_EXCEPTION(e);
   e.setTitle(QObject::tr("Logic Error"));
   e.setDetails(QObject::tr("Cannot call write result interface without implementation."));
   throw e;
}
