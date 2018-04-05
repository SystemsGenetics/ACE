#ifndef ACE_ANALYTIC_SINGLERUN_H
#define ACE_ANALYTIC_SINGLERUN_H
#include "ace_analytic_manager.h"
//



namespace Ace
{
   namespace Analytic
   {
      /*!
       * This is the session manager for class single process analytic runs. This is the 
       * last resort manager used if this program is being run normally with no MPI 
       * support or using multiple chunk processes that cannot communicate. This manager 
       * will simply run through all work blocks provided by the abstract analytic from 
       * beginning to end, running them through OpenCL if available or processing them on 
       * the CPU serially if not. 
       */
      class SingleRun : public Manager
      {
         Q_OBJECT
      public:
         SingleRun(quint16 type);
      protected slots:
         virtual void start() override final;
      private slots:
         void executeSerial();
      private:
         void next();
         /*!
          * Pointer to the abstract analytic serial object for processing work blocks using 
          * the CPU serially. If OpenCL is used then this is null. 
          */
         EAbstractAnalytic::Serial* _serial;
         /*!
          * Index to the next work block that must be processed. When this index reaches the 
          * work block size this manager has finished processing all blocks. 
          */
         int _next {0};
         /*!
          * The percent of blocks this manager has completed processing. 
          */
         int _percentComplete {0};
      };
   }
}



#endif
