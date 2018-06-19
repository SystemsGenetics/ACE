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
       * This is a simple analytic run that is used in single mode only for simple 
       * analytic types which do not make blocks. 
       */
      class SimpleRun : public AbstractRun
      {
         Q_OBJECT
      public:
         explicit SimpleRun(AbstractInput* base, QObject* parent = nullptr);
         virtual void addWork(std::unique_ptr<EAbstractAnalytic::Block>&& block) override final;
      private:
         /*!
          * The abstract input object this object uses to save the results produced by it. 
          */
         AbstractInput* _base;
      };
   }
}



#endif
