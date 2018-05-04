#include "ace_analytic_mpislave.h"
#include "ace_analytic_mpimaster.h"
#include "ace_analytic_serialrun.h"
#include "ace_analytic_openclrun.h"
#include "ace_qmpi.h"
#include "ace_settings.h"
#include "opencl_platform.h"
#include "opencl_device.h"
#include "eabstractanalytic_block.h"
#include "eexception.h"



using namespace std;
using namespace Ace::Analytic;
//






/*!
 * Constructs a new MPI slave manager with the given analytic type. 
 *
 * @param type The analytic type this manager will use. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Setup OpenCL, setup serial if OpenCL fails, connect this abstract run signal 
 *    to this manager's finish slot, and connect the MPI data received signal to 
 *    this manager's data received slot. 
 */
MPISlave::MPISlave(quint16 type):
   MPIBase(type),
   _mpi(QMPI::instance())
{
   connect(&_mpi,&QMPI::dataReceived,this,&MPISlave::dataReceived);
}






/*!
 * Properly shuts down the MPI system. 
 */
MPISlave::~MPISlave()
{
   QMPI::shutdown();
}






/*!
 * Implements the interface that tests if this abstract input is finished and 
 * received all result blocks for its analytic. This implementation is special 
 * because this is a slave node and is finished once it has received the 
 * termination signal and no longer has any blocks it is working on. 
 *
 * @return True if this abstract input is finished or false otherwise. 
 */
bool MPISlave::isFinished() const
{
   return _finished && _workSize == 0;
}






/*!
 *
 * @param type  
 *
 * @param platform  
 *
 * @param device  
 */
void MPISlave::mpiStart(Type type, int platform, int device)
{
   int code {ReadyAsSerial};
   switch (type)
   {
   case Type::Serial:
      setupSerial();
      break;
   case Type::OpenCL:
      if ( setupOpenCL(platform,device) )
      {
         code = ReadyAsOpenCL;
      }
      else
      {
         setupSerial();
      }
      break;
   }
   connect(_runner,&AbstractRun::finished,this,&AbstractManager::finish);
   unique_ptr<EAbstractAnalytic::Block> block {new EAbstractAnalytic::Block(code)};
   _mpi.sendData(0,block->toBytes());
}






/*!
 * Implements the interface that opens a new file set to write only and truncate 
 * with the given path. This implementation does nothing and returns a null pointer 
 * because it is a slave node and does not handle output. 
 *
 * @param path Unused path to file. 
 *
 * @return A null pointer. 
 */
QFile* MPISlave::addOutputFile(const QString& path)
{
   Q_UNUSED(path)
   return nullptr;
}






/*!
 * This interface opens a new data object with the given path, erasing any data the 
 * file may have contained and returning a pointer to the new data object. This 
 * implementation does nothing and returns a null pointer because it is a slave 
 * node and does not handle output. 
 *
 * @param path Unused path to data object file. 
 *
 * @param type Unused data object type. 
 *
 * @param system Unused system metadata for new data objects. 
 *
 * @return A null pointer. 
 */
Ace::DataObject* MPISlave::addOutputData(const QString& path, quint16 type, const EMetadata& system)
{
   Q_UNUSED(path)
   Q_UNUSED(type)
   Q_UNUSED(system)
   return nullptr;
}






/*!
 * Implements the interface that saves the given result block to its underlying 
 * analytic and assumes the order of indexes given is not sorted and random. This 
 * implementation simply sends the given result block back to the master node. 
 *
 * @param result The result block that is sent to the master node for saving. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Send the result block to the master node as byte data, deleting the result 
 *    block and decreasing this object's work size. 
 */
void MPISlave::saveResult(std::unique_ptr<EAbstractAnalytic::Block>&& result)
{
   _mpi.sendData(0,result->toBytes());
   result.reset();
   --_workSize;
}






/*!
 * Called when new data has been received from the master node. This takes the and 
 * processes it as a work block or termination code. 
 *
 * @param data The data received from the master node. 
 *
 * @param fromRank The process rank of the node which sent the data. This should 
 *                 always be rank 0 from the master node. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the given rank is not from the master node then throw an exception, else 
 *    go to the next step. 
 *
 * 2. If the given data is a special code then process the code, else process the 
 *    data as a work block. 
 */
void MPISlave::dataReceived(const QByteArray& data, int fromRank)
{
   if ( fromRank != 0 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Slave node received data from other slave node."));
      throw e;
   }
   int code {EAbstractAnalytic::Block::extractIndex(data)};
   if ( code < 0 )
   {
      processCode(code);
   }
   else
   {
      process(data);
   }
}






/*!
 * Processes a special code sent to this slave node by the master node. The only 
 * special code processed is to terminate signaling no more work blocks will be 
 * sent. 
 *
 * @param code The special code sent to this slave node by the master node. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the special code is to terminate then go to the next step, else throw an 
 *    exception. 
 *
 * 2. Set this object's finish condition to true. If this slave node is completely 
 *    finished then call this manager's finish slot. 
 */
void MPISlave::processCode(int code)
{
   if ( code == MPIMaster::Terminate )
   {
      _finished = true;
      if ( isFinished() )
      {
         finish();
      }
      return;
   }
   else
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Master MPI node sent unknown code %1 to slave.").arg(code));
      throw e;
   }
}






/*!
 * Processes a work block sent to this slave node by the master node by adding it 
 * to this abstract run for processing. 
 *
 * @param data The data containing a work block sent to this slave node by the 
 *             master node. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create a blank work block from this object's analytic reading in the given 
 *    data to it. If this object's analytic fails in creating a blank work block 
 *    then throw an exception. 
 *
 * 2. Increase this object's work size and add the work block to this object's 
 *    abstract run object. 
 */
void MPISlave::process(const QByteArray& data)
{
   unique_ptr<EAbstractAnalytic::Block> work {analytic()->makeWork()};
   if ( !work )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Analytic returned null work block pointer."));
      throw e;
   }
   work->fromBytes(data);
   ++_workSize;
   _runner->addWork(std::move(work));
}






/*!
 * Attempts to initialize an OpenCL run object for block processing for this 
 * manager. If successful sets this manager's abstract run pointer. 
 *
 * @param platform  
 *
 * @param device  
 *
 *
 * Steps of Operation: 
 *
 * 1. If the singleton settings object does not contain a valid OpenCL device 
 *    pointer then do nothing and exit, else go to the next step. 
 *
 * 2. Attempt to find an OpenCL index that is the nth device based off this slave 
 *    node's local process rank. All devices from all platforms are considered. If 
 *    there is not enough devices then no device is found. 
 *
 * 3. If an OpenCL device was found and this manager's analytic creates a valid 
 *    abstract OpenCL object then create a new OpenCL run object and set it to this 
 *    object's run pointer. 
 */
bool MPISlave::setupOpenCL(int platform, int device)
{
   if ( platform < 0
        || device < 0
        || platform >= OpenCL::Platform::size()
        || device >= OpenCL::Platform::get(platform)->deviceSize() )
   {
      ;//ERROR
   }
   bool ret {false};
   if ( EAbstractAnalytic::OpenCL* opencl = analytic()->makeOpenCL() )
   {
      _runner = new OpenCLRun(opencl,OpenCL::Platform::get(platform)->device(device),this,this);
      ret = true;
   }
   return ret;
}






/*!
 * Initializes this object's abstract run object as a serial run object if it has 
 * not already been set. If this manager's analytic fails creating a valid abstract 
 * serial object then an exception is thrown. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this object already has an abstract run object then do nothing and exit, 
 *    else go to the next step. 
 *
 * 2. If this manager's analytic creates a valid abstract serial object then create 
 *    a new serial run object and set it to this object's run pointer, else throw 
 *    an exception. 
 */
void MPISlave::setupSerial()
{
   if ( EAbstractAnalytic::Serial* serial = analytic()->makeSerial() )
   {
      _runner = new SerialRun(serial,this,this);
   }
   else
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Cannot run simple analytic in MPI mode."));
      throw e;
   }
}
