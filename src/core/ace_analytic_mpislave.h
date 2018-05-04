#ifndef ACE_ANALYTIC_MPISLAVE_H
#define ACE_ANALYTIC_MPISLAVE_H
#include "ace_analytic_mpibase.h"
#include "ace_analytic_abstractinput.h"
#include "ace_analytic.h"
//



namespace Ace
{
   namespace Analytic
   {
      /*!
       * This is the MPI slave manager. This manager is used for an MPI run with any 
       * process that is not rank 0 in other words a slave node. The sole function of 
       * this class is to process work blocks given to it by the master node and return 
       * the corresponding result blocks until a termination code is received. This class 
       * also sends a special code to the master node to signal when it is ready to start 
       * processing work blocks and if it is serial or accelerated. 
       */
      class MPISlave : public MPIBase, public AbstractInput
      {
         Q_OBJECT
      public:
         explicit MPISlave(quint16 type);
         virtual ~MPISlave() override final;
         virtual bool isFinished() const override final;
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
