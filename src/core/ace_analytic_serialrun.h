#ifndef ACE_ANALYTIC_SERIALRUN_H
#define ACE_ANALYTIC_SERIALRUN_H
#include "ace_analytic_run.h"
#include "ace_analytic.h"
#include "eabstractanalytic.h"
//



namespace Ace
{
   namespace Analytic
   {
      /*!
       */
      class SerialRun : public Run
      {
         Q_OBJECT
      public:
         explicit SerialRun(EAbstractAnalytic::Serial* serial, IOBase* base, QObject* parent = nullptr);
         virtual void addWork(std::unique_ptr<EAbstractAnalytic::Block>&& block) override final;
      private:
         /*!
          */
         EAbstractAnalytic::Serial* _serial;
         /*!
          */
         IOBase* _base;
      };
   }
}



#endif