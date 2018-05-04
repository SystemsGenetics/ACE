#ifndef ACE_ANALYTIC_MPIBASE_H
#define ACE_ANALYTIC_MPIBASE_H
#include "ace_analytic_abstractmanager.h"
//



namespace Ace
{
   namespace Analytic
   {
      /*!
       */
      class MPIBase : public AbstractManager
      {
         Q_OBJECT
      protected:
         /*!
          * Defines special codes passed between the master node and slave nodes. 
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
         /*!
          */
         enum Type
         {
            /*!
             */
            Serial
            /*!
             */
            ,OpenCL
         };
         MPIBase(quint16 type);
         virtual void mpiStart(Type type, int platform, int device);
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
          */
         bool _started {false};
      };
   }
}



#endif
