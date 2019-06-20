#ifndef ACE_ANALYTIC_ABSTRACTRUN_H
#define ACE_ANALYTIC_ABSTRACTRUN_H
#include <memory>
#include <QObject>
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
       * This represents the core of an analytic run itself. An implementation of this
       * class runs a specific type of analytic run, such as serial or OpenCL. An
       * interface is provided for adding work blocks that an implementation will run
       * and a signal for notifying when the analytic run is complete.
       */
      class AbstractRun : public QObject
      {
         Q_OBJECT
      public:
         /*!
          * This interface is called to add a work block to be processed by this abstract
          * run.
          *
          * @param block Block of work that is added to this abstract run for processing.
          */
         virtual void addWork(std::unique_ptr<EAbstractAnalyticBlock>&& block) = 0;
      public:
         explicit AbstractRun(QObject* parent = nullptr);
      signals:
         /*!
          * Signals that this abstract run has finished processing all blocks.
          */
         void finished();
      };
   }
}

#endif
