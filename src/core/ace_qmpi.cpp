#include "ace_qmpi.h"
#include <mpi.h>
#include "exception.h"



using namespace Ace;



/*!
 * This is a pointer to the single instance of this class. 
 */
QMPI* QMPI::_instance {nullptr};






/*!
 * This returns a reference to the singleton instance of this class. 
 *
 * @return Reference to the singleton instance of this class. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the instance pointer is null then create new instance of the class and 
 *    set pointer to its address. 
 *
 * 2. Return reference to instance pointed to by the instance pointer. 
 */
QMPI& QMPI::instance()
{
   if ( !_instance )
   {
      _instance = new QMPI;
   }
   return *_instance;
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
 * @param toRank  
 *
 * @param data  
 *
 *
 * Steps of Operation: 
 *
 * 1. Call the MPI system to send new byte data to the specified rank. If the 
 *    call was successful then control is returned else go to step 2. 
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
 * @param event  
 */
void QMPI::timerEvent(QTimerEvent* event)
{
   //TODO; poll if there are in MPI recv events
}






/*!
 * This is the private and only constructor for this class. It is private so no 
 * one except this class itself can make an instance of it thereby enforcing its 
 * singleton property. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Initialize the MPI system and if it failed throw an exception detailing 
 *    the error. 
 *
 * 2. Querying the MPI system for the size and rank. If the query failed throw 
 *    an exception detailing the error. 
 *
 * 3. Initialize a QObject timer event used for polling of new data received. 
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
   if ( MPI_Comm_size(MPI_COMM_WORLD,&_size) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("MPI_Comm_size Failed"));
      e.setDetails(tr("MPI_Comm_size failed."));
      throw e;
   }
   if ( MPI_Comm_rank(MPI_COMM_WORLD,&_rank) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("MPI_Comm_rank Failed"));
      e.setDetails(tr("MPI_Comm_rank failed."));
      throw e;
   }
   startTimer(_timerPeriod);
}

