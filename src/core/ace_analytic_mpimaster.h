#ifndef ACE_ANALYTIC_MPIMASTER_H
#define ACE_ANALYTIC_MPIMASTER_H
#include "ace_analytic_abstractmanager.h"
#include "ace_analytic_abstractinput.h"
//



namespace Ace
{
   namespace Analytic
   {
      /*!
       */
      class MPIMaster : public AbstractManager, public AbstractInput
      {
         Q_OBJECT
      public:
         /*!
          */
         enum Code
         {
            /*!
             */
            Terminate = -1
            /*!
             */
            ,ReadyAsSerial = -2
            /*!
             */
            ,ReadyAsACU = -3
         };
         MPIMaster(quint16 type);
         virtual ~MPIMaster() override final;
         virtual bool isFinished() const override final;
      protected:
         virtual int index() const override final;
         virtual void writeResult(std::unique_ptr<EAbstractAnalytic::Block>&& result) override final;
      private slots:
         void dataReceived(const QByteArray& data, int fromRank);
      private:
         void processCode(int code, int fromRank);
         void process(const QByteArray& data, int fromRank);
         void terminate(int rank);
         /*!
          */
         QMPI& _mpi;
         /*!
          */
         QVector<bool> _doneSlaves;
         /*!
          */
         int _nextWork {0};
         /*!
          */
         int _nextResult {0};
      };
   }
}



#endif
