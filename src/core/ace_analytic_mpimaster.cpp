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
 */
void MPIMaster::start()
{
   for (int t = 0; t < (Settings::instance().threadSize()*2) ;++t)
   {
      for (int i = 1; i < _mpi.size() ;++i)
      {
         if ( _nextWork >= analytic()->size() )
         {
            if ( t == 0 )
            {
               unique_ptr<EAbstractAnalytic::Block> work {new EAbstractAnalytic::Block(-1)};
               _mpi.sendData(i,work->toBytes());
            }
            else
            {
               return;
            }
         }
         unique_ptr<EAbstractAnalytic::Block> work {analytic()->makeBlock(_nextWork)};
         _mpi.sendData(i,work->toBytes());
      }
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
   unique_ptr<EAbstractAnalytic::Block> result
   {//
      analytic()->makeBlock(EAbstractAnalytic::Block::extractIndex(data))
   };
   result->fromBytes(data);
   saveResult(std::move(result));
   unique_ptr<EAbstractAnalytic::Block> work;
   if ( _nextWork < analytic()->size() )
   {
      work = makeWork(_nextWork++);
   }
   else if ( !_doneSlaves.at(fromRank - 1) )
   {
      work.reset(new EAbstractAnalytic::Block(-1));
      _doneSlaves[fromRank - 1] = true;
   }
   if ( work )
   {
      _mpi.sendData(fromRank,work->toBytes());
      work.reset();
   }
}
