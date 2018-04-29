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
         virtual bool isFinished() const override final;
      protected:
         virtual int index() const override final;
         virtual void writeResult(std::unique_ptr<EAbstractAnalytic::Block>&& result) override final;
      protected slots:
         virtual void start() override final;
         void process();
      private:
         void setupOpenCL();
         void setupSerial();
         /*!
          */
         Run* _runner {nullptr};
         /*!
          */
         bool _simple {false};
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
