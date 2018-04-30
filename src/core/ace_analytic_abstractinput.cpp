#include "ace_analytic_abstractinput.h"
#include "eabstractanalytic_block.h"
#include "eexception.h"



using namespace std;
using namespace Ace::Analytic;
//






/*!
 */
AbstractInput::~AbstractInput()
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
void AbstractInput::saveResult(std::unique_ptr<EAbstractAnalytic::Block>&& result)
{
   if ( !result )
   {
      result.reset(new EAbstractAnalytic::Block(index()));
   }
   if ( result->index() == index() )
   {
      writeResult(std::move(result));
      while ( _hopper.contains(index()) )
      {
         writeResult(unique_ptr<EAbstractAnalytic::Block>(_hopper.take(index())));
      }
   }
   else
   {
      EAbstractAnalytic::Block* block {result.release()};
      _hopper.insert(block->index(),block);
   }
}






/*!
 */
int AbstractInput::index() const
{
   return -1;
}






/*!
 *
 * @param result  
 */
void AbstractInput::writeResult(std::unique_ptr<EAbstractAnalytic::Block>&& result)
{
   result.reset();
   E_MAKE_EXCEPTION(e);
   e.setTitle(QObject::tr("Logic Error"));
   e.setDetails(QObject::tr("Cannot call write result interface without implementation."));
   throw e;
}
