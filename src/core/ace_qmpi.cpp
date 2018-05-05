#include "ace_qmpi.h"
#include "eexception.h"



using namespace Ace;
//



/*!
 * True if an instance of this class has already been deleted or false otherwise. 
 * This is used to make sure a second class is never deleted. 
 */
bool QMPI::_hasShutdown {false};
/*!
 * Pointer to the singleton instance of this class. 
 */
QMPI* QMPI::_instance {nullptr};






/*!
 * This returns a reference to the singleton instance of this class if shutdown of 
 * another instance has not occurred. If MPI was already initialized it simply 
 * returns a reference to the instance. 
 *
 * @return Reference to the singleton instance of this class. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If shutdown has already been called on another instance of this class throw 
 *    an exception about this fact. 
 *
 * 2. If the instance pointer is null then create new instance of the class and set 
 *    pointer to its address. 
 *
 * 3. Return reference to instance pointed to by the instance pointer. 
 */
QMPI& QMPI::instance()
{
   if ( _hasShutdown )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logical Error"));
      e.setDetails(tr("An instance of QMPI already existed and shutdown."));
      throw e;
   }
   if ( !_instance )
   {
      _instance = new QMPI;
   }
   return *_instance;
}






/*!
 * This deletes the singleton instance of this class thereby shutting down the MPI 
 * system. After calling this another instance cannot be initialized or referenced. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If an instance of this class exists in the static pointer delete it and set 
 *    the shutdown flag to true to prevent initializing another instance. 
 */
void QMPI::shutdown()
{
   if ( _instance )
   {
      delete _instance;
      _hasShutdown = true;
   }
}






/*!
 * Tests if this process is the master node (rank 0). 
 *
 * @return Returns true if this is the master node else returns false. 
 */
bool QMPI::isMaster() const
{
   return _rank == 0;
}






/*!
 * Returns world node size. 
 *
 * @return World node size. 
 */
int QMPI::size() const
{
   return _size;
}






/*!
 * Returns the world rank of this node. 
 *
 * @return World rank of this node. 
 */
int QMPI::rank() const
{
   return _rank;
}






/*!
 * Returns the local size representing the number of nodes that share local 
 * resources. 
 *
 * @return Size of nodes that share local resources. 
 */
int QMPI::localSize() const
{
   return _localSize;
}






/*!
 * Returns the local rank of this node to identify it within the number of other 
 * nodes that share its local resources. 
 *
 * @return Local rank of this node. 
 */
int QMPI::localRank() const
{
   return _localRank;
}






/*!
 * Called to send data to another node using the world MPI comm. This is 
 * asynchronous and returns immediately. 
 *
 * @param toRank The rank of the node to send this data to. 
 *
 * @param data The data that is sent to the node with the given rank. 
 */
void QMPI::sendData(int toRank, const QByteArray& data)
{
   sendData(MPI_COMM_WORLD,toRank,data);
}






/*!
 * Called to send data to another node using the local MPI comm. This is 
 * asynchronous and returns immediately. 
 *
 * @param toRank The rank of the process to send this data to. 
 *
 * @param data Stores the raw data that will be sent to the given process. 
 */
void QMPI::sendLocalData(int toRank, const QByteArray& data)
{
   sendData(_local,toRank,data);
}






/*!
 * Implements the QObject interface that is called whenever the timer event is 
 * fired. This function is used to poll for new data from the MPI system without 
 * blocking and firing signals for any new data received. 
 *
 * @param event Ignored Qt event data since this class only has a single timer 
 *              event active. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Iterate through all ranks, probing for new data received from the world and 
 *    local MPI comm channels for each rank. 
 */
void QMPI::timerEvent(QTimerEvent* event)
{
   Q_UNUSED(event)
   for (int i = 0; i < _size ;++i)
   {
      probe(MPI_COMM_WORLD,i);
      probe(_local,i);
   }
}






/*!
 * Constructs a new QMPI object initializing the MPI system it represents. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Initialize the MPI system. If it failed then set this object to a single 
 *    process state and exit, else go to the next step. 
 *
 * 2. Setup the world and local MPI comm channels and then start this object's qt 
 *    object timer for data received polling. 
 */
QMPI::QMPI()
{
   if ( MPI_Init(nullptr,nullptr) )
   {
      _failed = true;
      _size = 1;
      _rank = 0;
      _localSize = 1;
      _localRank = 0;
      return;
   }
   setupWorld();
   setupLocal();
   startTimer(_timerPeriod);
}






/*!
 * Frees local MPI resources and finalizes the MPI system. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If MPI initialization did not fail for this object then free the local MPI 
 *    comm and call the MPI finalize function. 
 */
QMPI::~QMPI()
{
   if ( !_failed )
   {
      MPI_Comm_free(&_local);
      MPI_Finalize();
   }
}






/*!
 * Probe the given MPI comm for any received data from the node with the given 
 * rank, emitting a data received signal if data is found. This does not block and 
 * returns immediately if there is no pending data to receive. 
 *
 * @param comm The MPI comm that is proved. This is either the world or local comm. 
 *
 * @param rank The from rank that is checked for new data received. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Probe to see if there is pending data for the given MPI comm from the given 
 *    rank. If probing fails then throw an exception, else if there is no pending 
 *    data then do nothing and exit, else go to the next step. 
 *
 * 2. Get the size of the pending data that is assumed to be of type MPI_CHAR and 
 *    then get the data itself. If getting the size of the data or getting the data 
 *    fails then throw an exception, else go to the next step. 
 *
 * 3. Emit a local data received or data received signal, depending on if the given 
 *    comm is the world or local one. 
 */
void QMPI::probe(MPI_Comm comm, int rank)
{
   int flag;
   MPI_Status status;
   if ( MPI_Iprobe(rank,0,comm,&flag,&status) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("MPI Failed"));
      e.setDetails(tr("MPI_Iprobe failed."));
      throw e;
   }
   if ( flag )
   {
      int count;
      if ( MPI_Get_count(&status,MPI_CHAR,&count) )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(tr("MPI Failed"));
         e.setDetails(tr("MPI_Get_count failed."));
         throw e;
      }
      QByteArray data;
      data.resize(count);
      if ( MPI_Recv(data.data(),count,MPI_CHAR,rank,0,comm,MPI_STATUS_IGNORE) )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(tr("MPI Failed"));
         e.setDetails(tr("MPI_Recv failed."));
         throw e;
      }
      if ( comm == _local )
      {
         emit localDataReceived(data,rank);
      }
      else
      {
         emit dataReceived(data,rank);
      }
   }
}






/*!
 * Send the given data to the given rank or local rank, depending on the given comm 
 * being world or local. This is asynchronous and returns immediately. 
 *
 * @param comm The MPI comm used to send the data. This is either world or local. 
 *
 * @param toRank The rank or local rank, depending on if the given comm is world or 
 *               local respectively, of the node to send this data to. 
 *
 * @param data The data that is sent to the node with the given rank or local rank. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Send the given data to the node with the given rank using the given MPI comm, 
 *    using the MPI function that does not block to do so. If sending fails then 
 *    throw an exception. 
 */
void QMPI::sendData(MPI_Comm comm, int toRank, const QByteArray& data)
{
   MPI_Request request;
   if ( MPI_Isend(data.data(),data.size(),MPI_CHAR,toRank,0,comm,&request) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("MPI Failed"));
      e.setDetails(tr("MPI_Send failed."));
      throw e;
   }
   MPI_Request_free(&request);
}






/*!
 * Sets up the world MPI comm by simply getting its size and this node's rank since 
 * MPI itself creates the world comm. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Get the world MPI comm size and this node's rank, saving it to this object. 
 *    If any MPI call fails then throw an exception. 
 */
void QMPI::setupWorld()
{
   if ( MPI_Comm_size(MPI_COMM_WORLD,&_size) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("MPI Failed"));
      e.setDetails(tr("MPI_Comm_size failed."));
      throw e;
   }
   if (MPI_Comm_rank(MPI_COMM_WORLD,&_rank) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("MPI Failed"));
      e.setDetails(tr("MPI_Comm_rank failed."));
      throw e;
   }
}






/*!
 * Sets up the local MPI comm which is determined by all nodes that share local 
 * resources, getting the local size and this node's local rank rank from it. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create the local comm and save it to this object, along with getting the 
 *    local size and this node's local rank from the created local comm. If any MPI 
 *    call fails then throw an exception. 
 */
void QMPI::setupLocal()
{
   if ( MPI_Comm_split_type(MPI_COMM_WORLD,MPI_COMM_TYPE_SHARED,0,MPI_INFO_NULL,&_local) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("MPI Failed"));
      e.setDetails(tr("MPI_Comm_split_type failed."));
      throw e;
   }
   if ( MPI_Comm_size(_local,&_localSize) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("MPI Failed"));
      e.setDetails(tr("MPI_Comm_size failed."));
      throw e;
   }
   if ( MPI_Comm_rank(_local,&_localRank) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("MPI Failed"));
      e.setDetails(tr("MPI_Comm_rank failed."));
      throw e;
   }
}
