#ifndef ACE_ANALYTIC_CUDARUN_THREAD_H
#define ACE_ANALYTIC_CUDARUN_THREAD_H
#include <QThread>
#include "ace_analytic_cudarun.h"
#include "global.h"
//



namespace Ace
{
   namespace Analytic
   {
      /*!
       * This is a single thread of execution used by its parent CUDA run class to
       * process work blocks into result blocks. The execution of this processing is done
       * on a separate thread to enhance speed. An abstract CUDA worker object is used
       * for actual processing using an CUDA device for acceleration.
       */
      class CUDARun::Thread : public QThread
      {
         Q_OBJECT
      public:
         explicit Thread(std::unique_ptr<EAbstractAnalyticCUDAWorker>&& worker, QObject* parent = nullptr);
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
          * Pointer to the abstract CUDA worker object used to process work blocks into
          * result blocks using CUDA acceleration.
          */
         EAbstractAnalyticCUDAWorker* _worker;
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
