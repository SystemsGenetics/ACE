#ifndef ACE_ANALYTIC_OPENCLRUN_THREAD_H
#define ACE_ANALYTIC_OPENCLRUN_THREAD_H
#include <QThread>
#include "ace_analytic_openclrun.h"
#include "eabstractanalytic_opencl.h"
//



namespace Ace
{
   namespace Analytic
   {
      /*!
       */
      class OpenCLRun::Thread : public QThread
      {
         Q_OBJECT
      public:
         explicit Thread(std::unique_ptr<EAbstractAnalytic::OpenCL::Block>&& engine, QObject* parent = nullptr);
         void execute(std::unique_ptr<EAbstractAnalytic::Block>&& block);
         std::unique_ptr<EAbstractAnalytic::Block> result();
      protected:
         virtual void run() override final;
      private:
         /*!
          */
         EAbstractAnalytic::OpenCL::Block* _engine;
         /*!
          */
         EAbstractAnalytic::Block* _work {nullptr};
         /*!
          */
         EAbstractAnalytic::Block* _result {nullptr};
      };
   }
}



#endif
