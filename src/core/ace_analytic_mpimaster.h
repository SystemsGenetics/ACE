#ifndef ACE_ANALYTIC_MPIMASTER_H
#define ACE_ANALYTIC_MPIMASTER_H
#include "ace_analytic_manager.h"
#include "ace_analytic_iobase.h"
//



namespace Ace
{
   namespace Analytic
   {
      /*!
       */
      class MPIMaster : public Manager, public IOBase
      {
         Q_OBJECT
      public:
         MPIMaster(quint16 type);
         virtual ~MPIMaster() override final;
         virtual bool isFinished() const override final;
      protected:
         virtual int index() const override final;
         virtual void writeResult(std::unique_ptr<EAbstractAnalytic::Block>&& result) override final;
      protected slots:
         virtual void start() override final;
      private slots:
         void dataReceived(const QByteArray& data, int fromRank);
      private:
         /*!
          */
         QMPI& _mpi;
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
