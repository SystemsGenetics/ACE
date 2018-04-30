#ifndef ACE_ANALYTIC_SERIALRUN_H
#define ACE_ANALYTIC_SERIALRUN_H
#include "ace_analytic_abstractrun.h"
#include "ace_analytic.h"
#include "eabstractanalytic.h"
//



namespace Ace
{
   namespace Analytic
   {
      /*!
       */
      class SerialRun : public AbstractRun
      {
         Q_OBJECT
      public:
         explicit SerialRun(EAbstractAnalytic::Serial* serial, AbstractInput* base, QObject* parent = nullptr);
         virtual void addWork(std::unique_ptr<EAbstractAnalytic::Block>&& block) override final;
      private:
         /*!
          */
         EAbstractAnalytic::Serial* _serial;
         /*!
          */
         AbstractInput* _base;
      };
   }
}



#endif
