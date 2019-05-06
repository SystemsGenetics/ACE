#ifndef ACE_ANALYTIC_SINGLE_H
#define ACE_ANALYTIC_SINGLE_H
#include "ace_analytic_abstractmanager.h"
#include "ace_analytic_abstractinput.h"
#include "ace_analytic.h"



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
       * This is the single process manager. This is the last resort manager used if
       * this program is being run normally with no MPI support or using multiple
       * chunk processes that cannot communicate. This manager will simply run through
       * all work blocks provided by the abstract analytic from beginning to end. This
       * manager uses OpenCL run, or serial run if OpenCL is not available, or as a
       * last resort simple run. This is the only manager type that can use the simple
       * run type.
       */
      class Single : public AbstractManager, public AbstractInput
      {
         Q_OBJECT
      public:
         virtual bool isFinished() const override final;
      public:
         explicit Single(quint16 type);
      protected:
         virtual int index() const override final;
         virtual void writeResult(std::unique_ptr<EAbstractAnalyticBlock>&& result) override final;
      protected slots:
         virtual void start() override final;
         void process();
      private:
         void setupCUDA();
         void setupOpenCL();
         void setupSerial();
      private:
         /*!
          * Pointer to this manager's abstract run object used for processing blocks.
          */
         AbstractRun* _runner {nullptr};
         /*!
          * True if this manager contains a simple run object or false otherwise.
          */
         bool _simple {false};
         /*!
          * Next work block index to be processed to maintain order.
          */
         int _nextWork {0};
         /*!
          * Next result block index to be processed to maintain order.
          */
         int _nextResult {0};
      };
   }
}

#endif
