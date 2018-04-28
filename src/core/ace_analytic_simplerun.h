#ifndef ACE_ANALYTIC_SIMPLERUN_H
#define ACE_ANALYTIC_SIMPLERUN_H
#include "ace_analytic_run.h"
#include "ace_analytic.h"
#include "global.h"
//



namespace Ace
{
   namespace Analytic
   {
      /*!
       */
      class SimpleRun : public Run
      {
         Q_OBJECT
      public:
         explicit SimpleRun(IOBase* base, QObject* parent = nullptr);
         virtual void addWork(std::unique_ptr<EAbstractAnalytic::Block>&& block) override final;
      private:
         /*!
          */
         IOBase* _base;
      };
   }
}



#endif
