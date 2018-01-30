#include <mpi.h>
#include "ace_qmpi.h"
#include "exception.h"



using namespace Ace;



QMPI* QMPI::_instance {nullptr};






QMPI& QMPI::instance()
{
   if ( !_instance )
   {
      _instance = new QMPI;
   }
   return *_instance;
}






int QMPI::size() const
{
   return _size;
}






int QMPI::rank() const
{
   return _rank;
}






bool QMPI::isMaster() const
{
   return _rank == 0;
}






bool QMPI::isSlave() const
{
   return _rank != 0;
}






void QMPI::sendData(int toRank, const QByteArray data)
{
   if ( MPI_Send(data.data(),data.size(),MPI_CHAR,toRank,0,MPI_COMM_WORLD) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("MPI_Send Failed"));
      e.setDetails(tr("MPI_Send failed."));
      throw e;
   }
}






QMPI::QMPI(QObject* parent):
   QObject(parent)
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
   startTimer(50);
}






void QMPI::timerEvent(QTimerEvent* event)
{
   //TODO; poll if there are in MPI recv events
}
