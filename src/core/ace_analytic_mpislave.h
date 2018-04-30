#ifndef ACE_ANALYTIC_MPISLAVE_H
#define ACE_ANALYTIC_MPISLAVE_H
#include "ace_analytic_abstractmanager.h"
#include "ace_analytic_abstractinput.h"
#include "ace_analytic.h"
//



namespace Ace
{
   namespace Analytic
   {
      /*!
       */
      class MPISlave : public AbstractManager, public AbstractInput
      {
         Q_OBJECT
      public:
         MPISlave(quint16 type);
         virtual ~MPISlave() override final;
         virtual bool isFinished() const override final;
      protected:
         virtual QFile* addOutputFile(const QString& path) override final;
         virtual Ace::DataObject* addOutputData(const QString& path, quint16 type, const EMetadata& system) override final;
         virtual void saveResult(std::unique_ptr<EAbstractAnalytic::Block>&& result) override final;
      protected slots:
         virtual void start() override final;
      private slots:
         void dataReceived(const QByteArray& data, int fromRank);
      private:
         void setupOpenCL();
         void setupSerial();
         /*!
          */
         QMPI& _mpi;
         /*!
          */
         AbstractRun* _runner {nullptr};
         /*!
          */
         bool _acu {false};
         /*!
          */
         int _workSize {0};
         /*!
          */
         bool _finished {false};
      };
   }
}



#endif
