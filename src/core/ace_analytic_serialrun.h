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
       * This is a serial analytic run that processes the blocks of an analytic in a 
       * serial fashion using the traditional CPU and host memory. This only uses a 
       * single thread because MPI should be used to use multiple CPU cores. 
       */
      class SerialRun : public AbstractRun
      {
         Q_OBJECT
      public:
         virtual void addWork(std::unique_ptr<EAbstractAnalytic::Block>&& block) override final;
      public:
         explicit SerialRun(EAbstractAnalytic::Serial* serial, AbstractInput* base, QObject* parent = nullptr);
      private:
         /*!
          * Pointer to the abstract analytic serial object used for processing work blocks 
          * into result blocks. 
          */
         EAbstractAnalytic::Serial* _serial;
         /*!
          * Pointer to the abstract input object used to save result blocks. 
          */
         AbstractInput* _base;
      };
   }
}



#endif
