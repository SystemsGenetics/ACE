#ifndef ACE_ANALYTIC_MPIMASTER_H
#define ACE_ANALYTIC_MPIMASTER_H
#include "ace_analytic_abstractmpi.h"
#include "ace_analytic_abstractinput.h"



/*!
 * This contains all private classes used internally by the ACE library and
 * should never be accessed by a developer using this library.
 */
namespace Ace
{
   /*!
    * This contains all classes related to running an analytic. This required its
    * own name space because of the immense complexity required for ACE to provide
    * an abstract interface for running analytic types in heterogeneous
    * environments.
    */
   namespace Analytic
   {
      /*!
       * This is the MPI master manager. This manager is used for an MPI run with the
       * node that has world rank 0. The main function of this class is to create new
       * work blocks and assign their execution to other slave nodes within the MPI
       * run, along with getting results back and saving them to the manager's
       * underlying analytic with the indexes being in order. Because this manager
       * does not process blocks itself it has no abstract run object of its own. Also
       * this manager does not start giving out work blocks to slave nodes until they
       * signal they are ready to process them.
       */
      class MPIMaster : public AbstractMPI, public AbstractInput
      {
         Q_OBJECT
      public:
         virtual bool isFinished() const override final;
      public:
         explicit MPIMaster(quint16 type);
         virtual ~MPIMaster() override final;
      protected:
         virtual int index() const override final;
         virtual void writeResult(std::unique_ptr<EAbstractAnalyticBlock>&& result) override final;
      private slots:
         void dataReceived(const QByteArray& data, int fromRank);
      private:
         void processCode(int code, int fromRank);
         void process(const QByteArray& data, int fromRank);
         void terminate(int rank);
      private:
         /*!
          * Reference to the singleton MPI object.
          */
         QMPI& _mpi;
         /*!
          * Next work block index to be processed to maintain order.
          */
         int _nextWork {0};
         /*!
          * Next result block index to be processed to maintain order.
          */
         int _nextResult {0};
      };
   }
}

#endif
