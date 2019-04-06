#ifndef ACE_ANALYTIC_CUDARUN_H
#define ACE_ANALYTIC_CUDARUN_H
#include <memory>
#include <QVector>
#include <QQueue>
#include "ace_analytic_abstractrun.h"
#include "ace_analytic.h"
#include "cuda_common.h"
#include "eabstractanalytic.h"
//



namespace Ace
{
   namespace Analytic
   {
      /*!
       * This is a CUDA analytic run that processes the blocks of an analytic using a
       * CUDA device with multiple threads using the device at once to process work
       * blocks. This is a complicated analytic run that has its own subclass
       * representing a thread that actually processes the work blocks into result
       * blocks.
       */
      class CUDARun : public AbstractRun
      {
         Q_OBJECT
      public:
         virtual void addWork(std::unique_ptr<EAbstractAnalyticBlock>&& block) override final;
      public:
         explicit CUDARun(EAbstractAnalyticCUDA* cuda, CUDA::Device* device, AbstractInput* base, QObject* parent = nullptr);
         virtual ~CUDARun() override final;
      private slots:
         void blockFinished(int index);
      private:
         class Thread;
      private:
         /*!
          * Pointer to the CUDA context used by this CUDA run object.
          */
         CUDA::Context* _context;
         /*!
          * Pointer to the abstract CUDA object used by this object to create abstract
          * workers for all its threads.
          */
         EAbstractAnalyticCUDA* _cuda;
         /*!
          * Pointer to the abstract input object used to save results.
          */
         AbstractInput* _base;
         /*!
          * Pointer list to all threads this object contains for processing work blocks into
          * result blocks.
          */
         QVector<Thread*> _threads;
         /*!
          * Queue of idle threads ready to execute another work block.
          */
         QQueue<Thread*> _idle;
      };
   }
}



#endif
