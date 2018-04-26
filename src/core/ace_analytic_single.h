#ifndef ACE_ANALYTIC_SINGLE_H
#define ACE_ANALYTIC_SINGLE_H
#include "ace_analytic_manager.h"
#include "ace_analytic_iobase.h"
#include "ace_analytic.h"
//



namespace Ace
{
   namespace Analytic
   {
      /*!
       * This is the session manager for class single process analytic runs. This is the 
       * last resort manager used if this program is being run normally with no MPI 
       * support or using multiple chunk processes that cannot communicate. This manager 
       * will simply run through all work blocks provided by the abstract analytic from 
       * beginning to end, running them through OpenCL if available or processing them on 
       * the CPU serially if not. 
       */
      class Single : public Manager, public IOBase
      {
         Q_OBJECT
      public:
         Single(quint16 type);
         virtual bool hasWork() const override final;
         virtual bool isFinished() const override final;
         virtual std::unique_ptr<EAbstractAnalytic::Block> makeWork() override final;
      protected:
         virtual int index() const override final;
         virtual void writeResult(std::unique_ptr<EAbstractAnalytic::Block>&& result) override final;
      protected slots:
         virtual void start() override final;
      private:
         /*!
          */
         Run* _runner;
         /*!
          */
         int _nextWork {0};
         /*!
          */
         int _nextResult {0};
         /*!
          * The percent of blocks this manager has completed processing. 
          */
         int _percentComplete {0};
      };
   }
}



#endif
