#ifndef ACE_ANALYTIC_OPENCLRUN_THREAD_H
#define ACE_ANALYTIC_OPENCLRUN_THREAD_H
#include <QThread>
#include "ace_analytic_openclrun.h"
#include "eabstractanalytic_opencl.h"
#include "global.h"
//



namespace Ace
{
   namespace Analytic
   {
      /*!
       * This is a single thread of execution used by its parent OpenCL run class to 
       * process work blocks into result blocks. The execution of this processing is done 
       * on a separate thread t enhance speed. An abstract OpenCL worker object is used 
       * for actual processing using an OpenCL device for acceleration. 
       */
      class OpenCLRun::Thread : public QThread
      {
         Q_OBJECT
      public:
         explicit Thread(std::unique_ptr<EAbstractAnalytic::OpenCL::Worker>&& worker, QObject* parent = nullptr);
         void execute(std::unique_ptr<EAbstractAnalytic::Block>&& block);
         std::unique_ptr<EAbstractAnalytic::Block> result();
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
         EAbstractAnalytic::OpenCL::Worker* _worker;
         /*!
          * Pointer to saved work block, if any, passed along to the separate thread for 
          * processing. 
          */
         EAbstractAnalytic::Block* _work {nullptr};
         /*!
          * Pointer to result block, if any, saved by this object's separate thread. 
          */
         EAbstractAnalytic::Block* _result {nullptr};
         /*!
          * Pointer to any exception that occurs on this object's separate thread while 
          * processing a work block. 
          */
         EException* _exception {nullptr};
      };
   }
}



#endif
