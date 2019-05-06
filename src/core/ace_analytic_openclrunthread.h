#ifndef ACE_ANALYTIC_OPENCLRUNTHREAD_H
#define ACE_ANALYTIC_OPENCLRUNTHREAD_H
#include <QThread>
#include "ace_analytic_openclrun.h"
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
       * This is a single thread of execution used by the OpenCL run class to process
       * work blocks into result blocks. The execution of this processing is done on a
       * separate thread t enhance speed. An abstract OpenCL worker object is used for
       * actual processing using an OpenCL device for acceleration.
       */
      class OpenCLRunThread : public QThread
      {
         Q_OBJECT
      public:
         explicit OpenCLRunThread(std::unique_ptr<EAbstractAnalyticOpenCLWorker>&& worker, QObject* parent = nullptr);
         void execute(std::unique_ptr<EAbstractAnalyticBlock>&& block);
         std::unique_ptr<EAbstractAnalyticBlock> result();
      signals:
         /*!
          */
         void blockFinished();
      protected:
         virtual void run() override final;
      private:
         /*!
          */
         QAtomicInteger<int> _switch {0};
         /*!
          * Pointer to the abstract OpenCL worker object used to process work blocks into
          * result blocks using OpenCL acceleration.
          */
         EAbstractAnalyticOpenCLWorker* _worker;
         /*!
          * Pointer to saved work block, if any, passed along to the separate thread for
          * processing.
          */
         EAbstractAnalyticBlock* _work {nullptr};
         /*!
          * Pointer to result block, if any, saved by this object's separate thread.
          */
         EAbstractAnalyticBlock* _result {nullptr};
         /*!
          * Pointer to any exception that occurs on this object's separate thread while
          * processing a work block.
          */
         EException* _exception {nullptr};
      };
   }
}

#endif
