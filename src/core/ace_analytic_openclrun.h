#ifndef ACE_ANALYTIC_OPENCLRUN_H
#define ACE_ANALYTIC_OPENCLRUN_H
#include <memory>
#include <QVector>
#include <QQueue>
#include "ace_analytic_abstractrun.h"
#include "ace_analytic.h"
#include "opencl.h"
#include "eabstractanalytic.h"
//



namespace Ace
{
   namespace Analytic
   {
      /*!
       * This is an OpenCL analytic run that processes the blocks of an analytic using an 
       * OpenCL device with multiple threads using the device at once to process work 
       * blocks. This is a complicated analytic run that has its own subclass 
       * representing a thread that actually processes the work blocks into result 
       * blocks. 
       */
      class OpenCLRun : public AbstractRun
      {
         Q_OBJECT
      public:
         explicit OpenCLRun(EAbstractAnalytic::OpenCL* opencl, OpenCL::Device* device, AbstractInput* base, QObject* parent = nullptr);
         virtual ~OpenCLRun() override final;
         virtual void addWork(std::unique_ptr<EAbstractAnalytic::Block>&& block) override final;
      private slots:
         void blockFinished(int index);
      private:
         class Thread;
         /*!
          * Pointer to the OpenCL context used by this OpenCL run object. 
          */
         OpenCL::Context* _context;
         /*!
          * Pointer to the abstract OpenCL object used by this object to create abstract 
          * workers for all its threads. 
          */
         EAbstractAnalytic::OpenCL* _opencl;
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
