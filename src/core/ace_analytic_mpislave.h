#ifndef ACE_ANALYTIC_MPISLAVE_H
#define ACE_ANALYTIC_MPISLAVE_H
#include "ace_analytic_manager.h"
#include "ace_analytic_iobase.h"
#include "ace_analytic.h"
//



namespace Ace
{
   namespace Analytic
   {
      /*!
       */
      class MPISlave : public Manager, public IOBase
      {
      public:
         MPISlave(quint16 type);
         virtual ~MPISlave() override final;
         virtual bool isFinished() const override final;
      protected:
         virtual QFile* addOutputFile(const QString& path) override final;
         virtual Ace::DataObject* addOutputData(const QString& path, quint16 type, const EMetadata& system) override final;
         virtual void saveResult(std::unique_ptr<EAbstractAnalytic::Block>&& result) override final;
      private slots:
         void dataReceived(const QByteArray& data, int fromRank);
      private:
         /*!
          */
         QMPI& _mpi;
         /*!
          */
         Run* _runner;
         /*!
          */
         int _workSize {0};
         /*!
          */
         bool _finished;
      };
   }
}



#endif
