#include "ace_analytic_mpimaster.h"
#include "ace_qmpi.h"
#include "ace_settings.h"
#include "eabstractanalytic_block.h"



using namespace std;
using namespace Ace::Analytic;
//






/*!
 *
 * @param type  
 */
MPIMaster::MPIMaster(quint16 type):
   Manager(type),
   _mpi(QMPI::instance()),
   _doneSlaves(_mpi.size() - 1,false)
{
   connect(&_mpi,&QMPI::dataReceived,this,&MPIMaster::dataReceived);
}






/*!
 */
MPIMaster::~MPIMaster()
{
   QMPI::shutdown();
}






/*!
 */
bool MPIMaster::isFinished() const
{
   return _nextResult >= analytic()->size();
}






/*!
 */
int MPIMaster::index() const
{
   return _nextResult;
}






/*!
 *
 * @param result  
 */
void MPIMaster::writeResult(std::unique_ptr<EAbstractAnalytic::Block>&& result)
{
   Manager::writeResult(std::move(result),_nextResult++);
   if ( isFinished() )
   {
      emit done();
      finish();
   }
}






/*!
 *
 * @param data  
 *
 * @param fromRank  
 */
void MPIMaster::dataReceived(const QByteArray& data, int fromRank)
{
   int index {EAbstractAnalytic::Block::extractIndex(data)};
   if ( index < 0 )
   {
      processCode(index,fromRank);
   }
   else
   {
      process(data,fromRank);
   }
}






/*!
 *
 * @param code  
 *
 * @param fromRank  
 */
void MPIMaster::processCode(int code, int fromRank)
{
   Settings& settings {Settings::instance()};
   int amount {settings.bufferSize()};
   switch (code)
   {
   case ReadyAsACU:
      amount += settings.threadSize();
   case ReadyAsSerial:
      break;
   default:
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(tr("Logic Error"));
         e.setDetails(tr("Slave MPI node sent unknown code %1 to master.").arg(code));
         throw e;
      }
   }
   if ( isFinished() )
   {
      terminate(fromRank);
   }
   else
   {
      while ( amount-- && _nextWork < analytic()->size() )
      {
         unique_ptr<EAbstractAnalytic::Block> work {makeWork(_nextWork++)};
         _mpi.sendData(fromRank,work->toBytes());
      }
   }
}






/*!
 *
 * @param data  
 *
 * @param fromRank  
 */
void MPIMaster::process(const QByteArray& data, int fromRank)
{
   unique_ptr<EAbstractAnalytic::Block> result {analytic()->makeResult()};
   result->fromBytes(data);
   saveResult(std::move(result));
   if ( _nextWork < analytic()->size() )
   {
      unique_ptr<EAbstractAnalytic::Block> work {makeWork(_nextWork++)};
      _mpi.sendData(fromRank,work->toBytes());
   }
   else
   {
      terminate(fromRank);
   }
}






/*!
 *
 * @param rank  
 */
void MPIMaster::terminate(int rank)
{
   unique_ptr<EAbstractAnalytic::Block> code {new EAbstractAnalytic::Block(Terminate)};
   _mpi.sendData(rank,code->toBytes());
}
