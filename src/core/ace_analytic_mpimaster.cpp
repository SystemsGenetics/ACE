#include "ace_analytic_mpimaster.h"
#include "ace_qmpi.h"
#include "ace_settings.h"
#include "eabstractanalytic_block.h"



using namespace std;
using namespace Ace::Analytic;
//






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
 */
void MPIMaster::writeResult(std::unique_ptr<EAbstractAnalytic::Block>&& result)
{
   // Write the result block to this manager's underlying analytic. If this manager 
   // is finished with all result blocks then emit the done signal and call the 
   // manager's finish slot. 
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
 */
void MPIMaster::dataReceived(const QByteArray& data, int fromRank)
{
   // Extract the index from the given data. If the index is less than 0 and a code 
   // then process it as a code, else process it as a result block. 
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
 */
void MPIMaster::processCode(int code, int fromRank)
{
   // Initialize the amount of blocks to send using the global setting buffer size. 
   Settings& settings {Settings::instance()};
   int amount {settings.bufferSize()};

   // Add to the amount of blocks to send to the slave node based off it being a 
   // serial or OpenCL type. 
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

   // If there is no more work blocks to be processed then send a terminate code to 
   // the slave node and exit, else go to the next step. 
   if ( _nextWork >= analytic()->size() )
   {
      terminate(fromRank);
   }

   // Else there are more work blocks to be processed. 
   else
   {
      // Send blocks in the amount determined or until there is no more work blocks to 
      // send. 
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
 */
void MPIMaster::process(const QByteArray& data, int fromRank)
{
   // Make a blank result block from this manager's analytic and load the given data 
   // into it. If the analytic fails making a blank result block then throw an 
   // exception, else go to the next step. 
   unique_ptr<EAbstractAnalytic::Block> result {analytic()->makeResult()};
   if ( !result )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Analytic returned null result block pointer."));
      throw e;
   }
   result->fromBytes(data);

   // Save the result block to this abstract input which will sort all result blocks 
   // using its internal sorting hopper. 
   saveResult(std::move(result));

   // If there are more work blocks to be processed then send the slave node a new 
   // work block to process, else send the terminate code to the slave node. 
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
 */
void MPIMaster::terminate(int rank)
{
   // Create a special data block with the terminate signal and send it to the slave 
   // node with the given rank. 
   unique_ptr<EAbstractAnalytic::Block> code {new EAbstractAnalytic::Block(Terminate)};
   _mpi.sendData(rank,code->toBytes());
}
