#ifndef ACE_ANALYTIC_ABSTRACTMPI_H
#define ACE_ANALYTIC_ABSTRACTMPI_H
#include "ace_analytic_abstractmanager.h"
//



namespace Ace
{
   namespace Analytic
   {
      /*!
       * This represents the common resources required for both master and slave MPI 
       * managers. The primary purpose of this class is to have each local rank 0 node 
       * assign the different resources of the system to all nodes which share them in 
       * the same local rank. The one interface this abstract class has is called for 
       * starting the MPI run of a slave node, assigning them a specific resource to use. 
       * The master node with local rank 0 skips itself in assigning resources since it 
       * does not execute blocks like a slave node. This class also defines special codes 
       * passed between master and slave nodes. 
       */
      class AbstractMPI : public AbstractManager
      {
         Q_OBJECT
      public:
         /*!
          * Defines the resource types that slave nodes can be assigned to execute as. 
          */
         enum Type
         {
            /*!
             * Defines the serial resource which causes a slave node to run in plain serial 
             * mode with no OpenCL device. 
             */
            Serial
            /*!
             * Defines the OpenCL resource which causes the slave node to run in accelerated 
             * OpenCL mode with a given platform and device index. 
             */
            ,OpenCL
         };
      protected:
         virtual void mpiStart(Type type, int platform, int device);
      protected:
         /*!
          * Defines special codes passed between the master and slave nodes. 
          */
         enum Code
         {
            /*!
             * Defines the terminate code which is sent to a slave node to signal it is done 
             * and should exit. 
             */
            Terminate = -1
            /*!
             * Defines the ready as serial code which is sent to the master node by a slave 
             * node to signal it is ready to execute work blocks using a serial run object. 
             */
            ,ReadyAsSerial = -2
            /*!
             * Defines the ready as OpenCL code which is sent to the master node by a slave 
             * node to signal it is ready to execute work blocks using an OpenCL run object. 
             */
            ,ReadyAsOpenCL = -3
         };
         explicit AbstractMPI(quint16 type);
      protected slots:
         virtual void start() override final;
      private slots:
         void localDataReceived(const QByteArray& data, int fromRank);
      private:
         void sendStart(int rank, Type type, int platform = -1, int device = -1);
         /*!
          * Reference to the singleton MPI object. 
          */
         QMPI& _mpi;
         /*!
          * True if this MPI manager has already been started as a slave node or false 
          * otherwise. This is used to make sure a node is not started more than once. 
          */
         bool _started {false};
      };
   }
}



#endif
