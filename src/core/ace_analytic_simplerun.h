#ifndef ACE_ANALYTIC_SIMPLERUN_H
#define ACE_ANALYTIC_SIMPLERUN_H
#include "ace_analytic_abstractrun.h"
#include "ace_analytic.h"
#include "global.h"
//



namespace Ace
{
   namespace Analytic
   {
      /*!
       */
      class SimpleRun : public AbstractRun
      {
         Q_OBJECT
      public:
         explicit SimpleRun(AbstractInput* base, QObject* parent = nullptr);
         virtual void addWork(std::unique_ptr<EAbstractAnalytic::Block>&& block) override final;
      private:
         /*!
          */
         AbstractInput* _base;
      };
   }
}



#endif
