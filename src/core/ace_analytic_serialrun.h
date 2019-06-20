#ifndef ACE_ANALYTIC_SERIALRUN_H
#define ACE_ANALYTIC_SERIALRUN_H
#include "ace_analytic_abstractrun.h"
#include "ace_analytic.h"
#include "eabstractanalytic.h"
//



/*!
 * This contains all private classes used internally by the ACE library and
 * should never be accessed by a developer using this library.
 */
namespace Ace
{
   /*!
    * This contains all classes related to running an analytic. This required its
    * own name space because of the immense complexity required for ACE to provide
    * an abstract interface for running analytic types in heterogeneous
    * environments.
    */
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
         virtual void addWork(std::unique_ptr<EAbstractAnalyticBlock>&& block) override final;
      public:
         explicit SerialRun(EAbstractAnalyticSerial* serial, AbstractInput* base, QObject* parent = nullptr);
      private:
         /*!
          * Pointer to the abstract analytic serial object used for processing work
          * blocks into result blocks.
          */
         EAbstractAnalyticSerial* _serial;
         /*!
          * Pointer to the abstract input object used to save result blocks.
          */
         AbstractInput* _base;
      };
   }
}

#endif
