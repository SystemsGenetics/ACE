#ifndef ACE_ANALYTIC_SIMPLERUN_H
#define ACE_ANALYTIC_SIMPLERUN_H
#include "ace_analytic_abstractrun.h"
#include "ace_analytic.h"
#include "global.h"



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
       * This is a simple analytic run that is used in single mode only for simple
       * analytic types which do not make blocks.
       */
      class SimpleRun : public AbstractRun
      {
         Q_OBJECT
      public:
         virtual void addWork(std::unique_ptr<EAbstractAnalyticBlock>&& block) override final;
      public:
         explicit SimpleRun(AbstractInput* base, QObject* parent = nullptr);
      private:
         /*!
          * The abstract input object this object uses to save the results produced by
          * it.
          */
         AbstractInput* _base;
      };
   }
}

#endif
