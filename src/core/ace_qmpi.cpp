#include "ace_qmpi.h"
#include <mpi.h>
#include "exception.h"



using namespace Ace;
//



/*!
 * Keeps track if an instance of this class has already been created then 
 * destroyed in shutdown. This is so a second instance is never made after the 
 * MPI system was shutdown with finalize. 
 */
bool QMPI::_hasShutdown {false};
/*!
 * This is a pointer to the single instance of this class. 
 */
QMPI* QMPI::_instance {nullptr};






/*!
 * This returns a reference to the singleton instance of this class if shutdown 
 * of another instance has not occurred. If MPI was already initialized it 
 * simply returns a reference to the instance. 
 *
 * @return Reference to the singleton instance of this class. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If shutdown has already been called on another instance of this class 
 *    throw an exception about this fact. 
 *
 * 2. If the instance pointer is null then create new instance of the class and 
 *    set pointer to its address. 
 *
 * 3. Return reference to instance pointed to by the instance pointer. 
 */
QMPI& QMPI::initialize()
{
   if ( _hasShutdown )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("MPI Initialize Failed"));
      e.setDetails(tr("An instance of QMPI already existed and as shutdown."));
      throw e;
   }
   if ( !_instance )
   {
      _instance = new QMPI;
   }
   return *_instance;
}






/*!
 * This deletes the singleton instance of this class thereby shutting down the 
 * MPI system. After calling this another instance cannot be initialized or 
 * referenced. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If an instance of this class exists in the static pointer delete it and 
 *    set the shutdown flag to true to prevent initializing another instance. 
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
 * Returns size of MPI run. 
 *
 * @return Size of MPI run. 
 */
int QMPI::size() const
{
   return _size;
}






/*!
 * Returns rank of this process to identify it within the MPI run. 
 *
 * @return Rank of this process. 
 */
int QMPI::rank() const
{
   return _rank;
}






/*!
 * Tests if this process is the master process (rank 0) of the MPI run. 
 *
 * @return Returns true if this process is the master else returns false. 
 */
bool QMPI::isMaster() const
{
   return _rank == 0;
}






/*!
 * This slot is called to send data to another process within the MPI run. 
 *
 * @param toRank The rank of the process to send this data to. 
 *
 * @param data Stores the raw data that will be sent to the given process. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Call the MPI system to send new byte data to the specified rank. If the 
 *    call was successful then end operation else go to step 2. 
 *
 * 2. Create exception detailing failure and throw it. 
 */
void QMPI::sendData(int toRank, const QByteArray& data)
{
   if ( MPI_Send(data.data(),data.size(),MPI_CHAR,toRank,0,MPI_COMM_WORLD) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("MPI_Send Failed"));
      e.setDetails(tr("MPI_Send failed."));
      throw e;
   }
}






/*!
 * This is the timer event function re implemented from the QObject class that 
 * is called whenever the timer event is fired. This function is used to poll 
 * for new data from other MPI processes without blocking and firing signals for 
 * any new data received. 
 *
 * @param event Ignored Qt event data since this class only has a single timer 
 *              event active. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Iterate through all ranks, excluding this process's rank, going to step 2 
 *    for each iteration. Once iteration is complete end operation. 
 *
 * 2. Probe for incoming data from given rank using MPI system. If MPI calls 
 *    fail go to step 5. If the probe indicates there is a pending block of data 
 *    proceed else go back to step 1. 
 *
 * 3. Receive the data block using the MPI system, storing it in a Qt byte 
 *    array. If the MPI system fails go to step 5. 
 *
 * 4. Emit the data received signal with the byte array and what process it came 
 *    from identified by rank. Go back to step 1. 
 *
 * 5. Throw an exception detailing the error that occurred. 
 */
void QMPI::timerEvent(QTimerEvent* event)
{
   Q_UNUSED(event)
   for (int i = 0; i < _size ;++i)
   {
      if ( i != _rank )
      {
         int flag;
         MPI_Status status;
         if ( MPI_Iprobe(i,0,MPI_COMM_WORLD,&flag,&status) )
         {
            E_MAKE_EXCEPTION(e);
            e.setTitle(tr("MPI_Iprobe Failed"));
            e.setDetails(tr("MPI_Iprobe failed."));
            throw e;
         }
         if ( flag )
         {
            int count;
            if ( MPI_Get_count(&status,MPI_CHAR,&count) )
            {
               E_MAKE_EXCEPTION(e);
               e.setTitle(tr("MPI_Get_count Failed"));
               e.setDetails(tr("MPI_Get_count failed."));
               throw e;
            }
            QByteArray data;
            data.resize(count);
            if ( MPI_Recv(data.data(),count,MPI_CHAR,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE) )
            {
               E_MAKE_EXCEPTION(e);
               e.setTitle(tr("MPI_Recv Failed"));
               e.setDetails(tr("MPI_Recv failed."));
               throw e;
            }
            emit dataReceived(data,i);
         }
      }
   }
}






/*!
 * This is the private and only constructor for this class. It is private so no 
 * one except this class itself can make an instance of it thereby enforcing its 
 * singleton property. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Initialize the MPI system. If it failed go to step 4. 
 *
 * 2. Query the MPI system for the size and rank. If the query failed go to step 
 *    4. 
 *
 * 3. Initialize a QObject timer event used for polling of new data received and 
 *    end operation. 
 *
 * 4. Throw an exception detailing the error that occurred. 
 */
QMPI::QMPI()
{
   if ( MPI_Init(nullptr,nullptr) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("MPI_Init Failed"));
      e.setDetails(tr("MPI_Init failed."));
      throw e;
   }
   if ( MPI_Comm_size(MPI_COMM_WORLD,&_size) || MPI_Comm_rank(MPI_COMM_WORLD,&_rank) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("MPI_Comm Failed"));
      e.setDetails(tr("MPI_Comm failed."));
      throw e;
   }
   startTimer(_timerPeriod);
}






/*!
 * This simply calls the finalize function for MPI to shutdown the system. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Call MPI finalize. 
 */
QMPI::~QMPI()
{
   MPI_Finalize();
}
