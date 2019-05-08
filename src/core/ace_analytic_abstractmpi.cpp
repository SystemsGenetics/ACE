#include "ace_analytic_abstractmpi.h"
#include <QDataStream>
#include "ace_qmpi.h"
#include "ace_settings.h"
#include "cuda_device.h"
#include "opencl_platform.h"
#include "opencl_device.h"
#include "eexception.h"
#include "edebug.h"



namespace Ace
{
namespace Analytic
{






/*!
 * This interface is called to start this MPI manager as a slave node with the
 * given resource type and optional platform and device index. The platform and
 * device index is only used if the given resource type is OpenCL. The default
 * implementation throws an exception because if this is called it should be
 * implemented.
 *
 * @param type The resource type this MPI slave node runs as.
 *
 * @param platform The optional platform index for the resource type if it is
 *                 OpenCL.
 *
 * @param device The optional device index for the resource type if it is
 *               OpenCL.
 */
void AbstractMPI::mpiStart(Type type, int platform, int device)
{
   // Add the debug header.
   EDEBUG_FUNC(this,type,platform,device);

   // Throw an exception.
   Q_UNUSED(type)
   Q_UNUSED(platform)
   Q_UNUSED(device)
   E_MAKE_EXCEPTION(e);
   e.setTitle(tr("Logic Error"));
   e.setDetails(tr("Cannot call MPI start interface without implementation."));
   throw e;
}






/*!
 * Constructs a new abstract MPI object with the given analytic type.
 *
 * @param type Analytic type that is used for this manager's analytic run.
 */
AbstractMPI::AbstractMPI(quint16 type)
   :
   AbstractManager(type),
   _mpi(QMPI::instance())
{
   EDEBUG_FUNC(this,type);
   connect(&_mpi,&QMPI::localDataReceived,this,&AbstractMPI::localDataReceived);
}






/*!
 * Implements the interface that is called once to begin the analytic run for
 * this manager after all argument input has been set. This implementation
 * assigns the resources of the local system only if this MPI node is local rank
 * 0.
 */
void AbstractMPI::start()
{
   // Add the debug header.
   EDEBUG_FUNC(this);

   // Start MPI listening for incoming messages.
   _mpi.start();

   // If this node's local rank is not 0 then do nothing and exit, else go to the
   // next step.
   if ( _mpi.localRank() == 0 )
   {
      // If this is not the master node then assign a special rank iterator to 0, else
      // assign it to 1.
      int i {0};
      if ( _mpi.isMaster() )
      {
         analytic()->initializeOutputs();
         ++i;
      }

      // If the global settings returns a valid CUDA device pointer then iterate through
      // all available CUDA devices and assign one per local rank until there are no
      // more devices to assign or no more local ranks to assign them to.
      if ( Settings::instance().cudaDevicePointer() )
      {
         for (int device = 0; device < CUDA::Device::size() ;++device)
         {
            if ( i >= _mpi.localSize() )
            {
               return;
            }
            sendStart(i++,Type::CUDA,0,device);
         }
      }

      // If the global settings returns a valid OpenCL device pointer then iterate
      // through all available OpenCL devices and assign one per local rank until there
      // are no more devices to assign or no more local ranks to assign them to.
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

      // Assign any remaining local ranks as serial.
      while ( i < _mpi.localSize() )
      {
         sendStart(i++,Serial);
      }
   }
}






/*!
 * Called when this node received new data from the local MPI comm. The only
 * purpose for this is to send instructions for starting this node as a slave
 * with a given resource encoded within the data received.
 *
 * @param data The data received from a slave node.
 *
 * @param fromRank The rank of the slave node process that sent the received
 *                 data.
 */
void AbstractMPI::localDataReceived(const QByteArray& data, int fromRank)
{
   // Add the debug header.
   EDEBUG_FUNC(this,data,fromRank);

   // If this node has already been started then throw an exception, else go to the
   // next step.
   Q_UNUSED(fromRank)
   if ( _started )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Received MPI start signal more than once."));
      throw e;
   }

   // Extract the information contained in the given data received and call the MPI
   // start interface.
   QDataStream stream(data);
   int type;
   int platform;
   int device;
   stream >> type >> platform >> device;
   _started = true;
   mpiStart(static_cast<Type>(type),platform,device);
}






/*!
 * Sends a start message to the given local rank to use the given resource and
 * optional platform and device indexes.
 *
 * @param rank The local rank that this start message is sent to.
 *
 * @param type The resource type that is being assigned to the node with the
 *             given local rank.
 *
 * @param platform Optional platform index that is being assigned to the node
 *                 with the given local rank.
 *
 * @param device Optional device index that is being assigned to the node with
 *               the given local rank.
 */
void AbstractMPI::sendStart(int rank, Type type, int platform, int device)
{
   // Add the debug header.
   EDEBUG_FUNC(this,rank,type,platform,device);

   // Encode a byte array with the resource type, platform index, and device index,
   // sending the byte array to the node with the given local rank.
   QByteArray data;
   QDataStream stream(&data,QIODevice::WriteOnly);
   stream << static_cast<int>(type) << platform << device;
   _mpi.sendLocalData(rank,data);
}

}
}
