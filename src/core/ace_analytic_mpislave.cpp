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
#include "edebug.h"
#include "emetadata.h"



using namespace std;
using namespace Ace::Analytic;
//






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
   EDEBUG_FUNC(this)

   return _finished && _workSize == 0;
}






/*!
 * Constructs a new MPI slave manager with the given analytic type. 
 *
 * @param type The analytic type this manager will use. 
 */
MPISlave::MPISlave(quint16 type):
   AbstractMPI(type),
   _mpi(QMPI::instance())
{
   EDEBUG_FUNC(this,type)

   connect(&_mpi,&QMPI::dataReceived,this,&MPISlave::dataReceived);
}






/*!
 * Properly shuts down the MPI system. 
 */
MPISlave::~MPISlave()
{
   EDEBUG_FUNC(this)

   QMPI::shutdown();
}






/*!
 * Implements the interface that is called to start this MPI manager as a slave 
 * node with the given resource type and optional platform and device index. 
 *
 * @param type The resource type this slave node will use for block processing. 
 *
 * @param platform The optional platform index this slave node will use if its 
 *                 resource type is OpenCL. 
 *
 * @param device The optional device index this slave node will use if its resource 
 *               type is OpenCL. 
 */
void MPISlave::mpiStart(Type type, int platform, int device)
{
   EDEBUG_FUNC(this,type,platform,device)

   // Initialize a serial or OpenCL abstract run object depending on whether the 
   // given resource type is serial or OpenCL, respectively. If initializing OpenCL 
   // fails then fall back to initializing a serial run object. 
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
   default:
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(tr("Invalid MPI Starting type."));
         e.setDetails(tr("The given type for starting this MPI slave is invalid."));
         throw e;
      }
   }

   // Connect the runner object finished signal to this abstract manager's finish 
   // slot and send a special code to the master node signaling this slave node is 
   // ready to process work blocks with the resource type it was given. 
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
   EDEBUG_FUNC(this,path)

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
   EDEBUG_FUNC(this,path,type,system)

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
 */
void MPISlave::saveResult(std::unique_ptr<EAbstractAnalytic::Block>&& result)
{
   EDEBUG_FUNC(this,result.get())

   // Send the result block to the master node as byte data, deleting the result 
   // block and decreasing this object's work size. 
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
 */
void MPISlave::dataReceived(const QByteArray& data, int fromRank)
{
   EDEBUG_FUNC(this,&data,fromRank)

   // If the given rank is not from the master node then throw an exception, else go 
   // to the next step. 
   if ( fromRank != 0 )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Slave node received data from other slave node."));
      throw e;
   }

   // If the given data is a special code then process the code, else process the 
   // data as a work block. 
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
 */
void MPISlave::processCode(int code)
{
   EDEBUG_FUNC(this,code)

   // If the special code is to terminate then go to the next step, else throw an 
   // exception. 
   if ( code == MPIMaster::Terminate )
   {
      // Set this object's finish condition to true. If this slave node is completely 
      // finished then call this manager's finish slot. 
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
 */
void MPISlave::process(const QByteArray& data)
{
   EDEBUG_FUNC(this,&data)

   // Create a blank work block from this object's analytic reading in the given data 
   // to it. If this object's analytic fails in creating a blank work block then 
   // throw an exception. 
   unique_ptr<EAbstractAnalytic::Block> work {analytic()->makeWork()};
   if ( !work )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Analytic returned null work block pointer."));
      throw e;
   }
   work->fromBytes(data);

   // Increase this object's work size and add the work block to this object's 
   // abstract run object. 
   ++_workSize;
   _runner->addWork(std::move(work));
}






/*!
 * Initializes a new OpenCL run object for block processing of this slave node 
 * manager, returning true if this was successful or false otherwise. If the given 
 * platform and device indexes are not a valid OpenCL device then an exception is 
 * thrown. 
 *
 * @param platform  
 *
 * @param device  
 *
 * @return True if a valid OpenCL run object was created at set to this object or 
 *         false otherwise. 
 */
bool MPISlave::setupOpenCL(int platform, int device)
{
   EDEBUG_FUNC(this,platform,device)

   // If the given platform and device indexes are invalid then throw an exception, 
   // else go to the next step. 
   if ( platform < 0
        || device < 0
        || platform >= OpenCL::Platform::size()
        || device >= OpenCL::Platform::get(platform)->deviceSize() )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Invalid Argument"));
      e.setDetails(tr("OpenCL device %1:%2 does not exist.").arg(platform).arg(device));
      throw e;
   }

   // Attempt to create an abstract analytic OpenCL object from this manager's 
   // analytic. If a valid one is returned then create a new OpenCL run object, set 
   // it to this object, and return true. Else if no valid one is returned then 
   // return false. 
   bool ret {false};
   if ( EAbstractAnalytic::OpenCL* opencl = analytic()->makeOpenCL() )
   {
      _runner = new OpenCLRun(opencl,OpenCL::Platform::get(platform)->device(device),this,this);
      ret = true;
   }
   return ret;
}






/*!
 * Initializes a new serial run object for block processing of this slave node 
 * manager. If this manager's analytic fails creating a valid abstract serial 
 * object then an exception is thrown. 
 */
void MPISlave::setupSerial()
{
   EDEBUG_FUNC(this)

   // Create a new abstract serial object from this manager's analytic. If creating a 
   // new abstract serial object fails then throw an exception, else go to the next 
   // step. 
   if ( EAbstractAnalytic::Serial* serial = analytic()->makeSerial() )
   {
      // Create a new serial run object and set to as this object's runner. 
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
