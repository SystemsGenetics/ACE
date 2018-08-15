#ifndef ACE_ANALYTIC_MPISLAVE_H
#define ACE_ANALYTIC_MPISLAVE_H
#include "ace_analytic_abstractmpi.h"
#include "ace_analytic_abstractinput.h"
#include "ace_analytic.h"
//



namespace Ace
{
   namespace Analytic
   {
      /*!
       * This is the MPI slave manager. This manager is used for an MPI run with any node 
       * that is not rank 0 and by definition a slave node. The sole function of this 
       * class is to process work blocks given to it by the master node and return the 
       * corresponding result blocks until a termination code is received. This class 
       * also sends a special code to the master node to signal when it is ready to start 
       * processing work blocks and if it is serial or accelerated. This special signal 
       * to the master node is not sent until it is given the resource type it will use 
       * by having the interface mpi start called from its inherited abstract class. 
       */
      class MPISlave : public AbstractMPI, public AbstractInput
      {
         Q_OBJECT
      public:
         virtual bool isFinished() const override final;
      public:
         explicit MPISlave(quint16 type);
         virtual ~MPISlave() override final;
      protected:
         virtual void mpiStart(Type type, int platform, int device) override final;
         virtual QFile* addOutputFile(const QString& path) override final;
         virtual Ace::DataObject* addOutputData(const QString& path, quint16 type, const EMetadata& system) override final;
         virtual void saveResult(std::unique_ptr<EAbstractAnalytic::Block>&& result) override final;
      private slots:
         void dataReceived(const QByteArray& data, int fromRank);
      private:
         void processCode(int code);
         void process(const QByteArray& data);
         bool setupOpenCL(int platform, int device);
         void setupSerial();
         /*!
          * Reference to the singleton MPI object. 
          */
         QMPI& _mpi;
         /*!
          * Pointer to the abstract run object used to process work blocks. 
          */
         AbstractRun* _runner {nullptr};
         /*!
          * The total number of work blocks this slave node is currently processing. 
          */
         int _workSize {0};
         /*!
          * True if this slave node has received the terminate signal from the master node 
          * or false otherwise. 
          */
         bool _finished {false};
      };
   }
}



#endif
