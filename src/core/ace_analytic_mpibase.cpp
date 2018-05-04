#include "ace_analytic_mpibase.h"
#include <QDataStream>
#include "ace_qmpi.h"
#include "ace_settings.h"
#include "opencl_platform.h"
#include "opencl_device.h"
#include "eexception.h"



using namespace Ace::Analytic;
//






/*!
 *
 * @param type Analytic type that is used for this manager's analytic run. 
 */
MPIBase::MPIBase(quint16 type):
   AbstractManager(type),
   _mpi(QMPI::instance())
{
   connect(&_mpi,&QMPI::localDataReceived,this,&MPIBase::localDataReceived);
}






/*!
 *
 * @param type  
 *
 * @param platform  
 *
 * @param device  
 */
void MPIBase::mpiStart(Type type, int platform, int device)
{
   Q_UNUSED(type)
   Q_UNUSED(platform)
   Q_UNUSED(device)
   E_MAKE_EXCEPTION(e);
   e.setTitle(tr("Logic Error"));
   e.setDetails(tr("Cannot call mpi start interface without implementation."));
   throw e;
}






/*!
 * This interface is called once to begin the analytic run for this manager after 
 * all argument input has been set. The default implementation does nothing. 
 */
void MPIBase::start()
{
   if ( _mpi.localRank() == 0 )
   {
      int i {0};
      if ( _mpi.isMaster() )
      {
         ++i;
      }
      if ( Settings::instance().openCLDevicePointer() )
      {
         for (int platform = 0; ( platform < OpenCL::Platform::size() ) ;++platform)
         {
            for (int device = 0; device < OpenCL::Platform::get(platform)->deviceSize() ;++device)
            {
               if ( i >= _mpi.localSize() )
               {
                  return;
               }
               sendStart(i++,Type::OpenCL,platform,device);
            }
         }
      }
      while ( i < _mpi.localSize() )
      {
         sendStart(i++,Serial);
      }
   }
}






/*!
 *
 * @param data The data received from a slave node. 
 *
 * @param fromRank The rank of the slave node process that sent the received data. 
 */
void MPIBase::localDataReceived(const QByteArray& data, int fromRank)
{
   Q_UNUSED(fromRank)
   if ( _started )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Received MPI slave start signal more than once."));
      throw e;
   }
   QDataStream stream(data);
   int type;
   int platform;
   int device;
   stream >> type >> platform >> device;
   _started = true;
   mpiStart(static_cast<Type>(type),platform,device);
}






/*!
 *
 * @param rank  
 *
 * @param type  
 *
 * @param platform  
 *
 * @param device  
 */
void MPIBase::sendStart(int rank, Type type, int platform, int device)
{
   QByteArray data;
   QDataStream stream(&data,QIODevice::WriteOnly);
   stream << static_cast<int>(type) << platform << device;
   _mpi.sendLocalData(rank,data);
}
