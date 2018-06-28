#include "ace_analytic_mpimaster.h"
#include "ace_qmpi.h"
#include "ace_settings.h"
#include "eabstractanalytic_block.h"



using namespace std;
using namespace Ace::Analytic;
//






/*!
 * Constructs a new MPI master manager with the given analytic type. 
 *
 * @param type The analytic type this manager will use. 
 */
MPIMaster::MPIMaster(quint16 type):
   AbstractMPI(type),
   _mpi(QMPI::instance())
{
   connect(&_mpi,&QMPI::dataReceived,this,&MPIMaster::dataReceived);
}






/*!
 * Properly shuts down the MPI system. 
 */
MPIMaster::~MPIMaster()
{
   QMPI::shutdown();
}






/*!
 * Implements the interface that tests if this abstract input is finished and 
 * received all result blocks for its analytic. 
 *
 * @return True if this abstract input is finished or false otherwise. 
 */
bool MPIMaster::isFinished() const
{
   return _nextResult >= analytic()->size();
}






/*!
 * Implements the interface that returns the next expected result block index to 
 * maintain order of result blocks. 
 *
 * @return The next expected result block index to maintain order. 
 */
int MPIMaster::index() const
{
   return _nextResult;
}






/*!
 * Implements the interface that is called to save the given result block to the 
 * underlying analytic and it can be assumed that the index order is maintained 
 * from least to greatest. 
 *
 * @param result The result block that is processed by this manager's analytic. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Write the result block to this manager's underlying analytic. If this manager 
 *    is finished with all result blocks then emit the done signal and call the 
 *    manager's finish slot. 
 */
void MPIMaster::writeResult(std::unique_ptr<EAbstractAnalytic::Block>&& result)
{
   AbstractManager::writeResult(std::move(result),_nextResult++);
   if ( isFinished() )
   {
      emit done();
      finish();
   }
}






/*!
 * Called when new data has been received from one of the slave nodes. This takes 
 * the data and processes it depending on what it is. 
 *
 * @param data The data received from a slave node. 
 *
 * @param fromRank The rank of the slave node process that sent the received data. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Extract the index from the given data. If the index is less than 0 and a code 
 *    then process it as a code, else process it as a result block. 
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
 * Processes a special code sent to this master node by a slave node. The only 
 * special codes processes are for the slave node signaling it is ready to process 
 * blocks. 
 *
 * @param code The special code sent to this master node by a slave node. 
 *
 * @param fromRank The process rank of the slave node that sent the code. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given code is not a ready signal and therefore unknown then throw an 
 *    exception, else go to the next step. 
 *
 * 2. If there is no more work blocks to be processed then send a terminate code to 
 *    the slave node and exit, else go to the next step. 
 *
 * 3. Determine the amount of blocks to be initially sent to the slave node, based 
 *    off the buffer size setting and thread size if the slave node is running in 
 *    OpenCL mode. 
 *
 * 4. Send blocks in the amount determined or until there is no more work blocks to 
 *    send. 
 */
void MPIMaster::processCode(int code, int fromRank)
{
   Settings& settings {Settings::instance()};
   int amount {settings.bufferSize()};
   switch (code)
   {
   case ReadyAsOpenCL:
      amount += settings.threadSize() + 1;
      break;
   case ReadyAsSerial:
      ++amount;
      break;
   default:
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(tr("Logic Error"));
         e.setDetails(tr("Slave MPI node sent unknown code %1 to master.").arg(code));
         throw e;
      }
   }
   if ( _nextWork >= analytic()->size() )
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
 * Processes a result block sent to this master node by a slave node by saving it 
 * to this abstract input's hopper for sorting and checking if it is done. 
 *
 * @param data The data containing a result block sent to this master node by a 
 *             slave node. 
 *
 * @param fromRank The process rank of the slave node that sent the result block. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Make a blank result block from this manager's analytic and load the given 
 *    data into it. If the analytic fails making a blank result block then throw an 
 *    exception, else go to the next step. 
 *
 * 2. Save the result block to this abstract input which will sort all result 
 *    blocks using its internal sorting hopper. 
 *
 * 3. If there are more work blocks to be processed then send the slave node a new 
 *    work block to process, else send the terminate code to the slave node. 
 */
void MPIMaster::process(const QByteArray& data, int fromRank)
{
   unique_ptr<EAbstractAnalytic::Block> result {analytic()->makeResult()};
   if ( !result )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Analytic returned null result block pointer."));
      throw e;
   }
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
 * Sends a terminate code to the slave node with the given process rank. 
 *
 * @param rank Process rank of the slave node who is sent the terminate code. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a special data block with the terminate signal and send it to the 
 *    slave node with the given rank. 
 */
void MPIMaster::terminate(int rank)
{
   unique_ptr<EAbstractAnalytic::Block> code {new EAbstractAnalytic::Block(Terminate)};
   _mpi.sendData(rank,code->toBytes());
}
