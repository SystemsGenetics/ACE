#ifndef ACE_ANALYTIC_OPENCLRUN_H
#define ACE_ANALYTIC_OPENCLRUN_H
#include <memory>
#include <QVector>
#include <QQueue>
#include <QThread>
#include "ace_analytic_abstractrun.h"
#include "ace_analytic.h"
#include "opencl.h"
#include "eabstractanalytic.h"



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
       * This is an OpenCL analytic run that processes the blocks of an analytic using
       * an OpenCL device with multiple threads using the device at once to process
       * work blocks. This is a complicated analytic run that has its own separate
       * class representing a thread that actually processes the work blocks into
       * result blocks.
       */
      class OpenCLRun : public AbstractRun
      {
         Q_OBJECT
      public:
         virtual void addWork(std::unique_ptr<EAbstractAnalyticBlock>&& block) override final;
      public:
         explicit OpenCLRun(EAbstractAnalyticOpenCL* opencl, OpenCL::Device* device, AbstractInput* base, QObject* parent = nullptr);
         virtual ~OpenCLRun() override final;
      private slots:
         void blockFinished(int index);
      private:
         /*!
          * Pointer to the OpenCL context used by this OpenCL run object.
          */
         OpenCL::Context* _context;
         /*!
          * Pointer to the abstract OpenCL object used by this object to create abstract
          * workers for all its threads.
          */
         EAbstractAnalyticOpenCL* _opencl;
         /*!
          * Pointer to the abstract input object used to save results.
          */
         AbstractInput* _base;
         /*!
          * Pointer list to all threads this object contains for processing work blocks
          * into result blocks.
          */
         QVector<OpenCLRunThread*> _threads;
         /*!
          * Queue of idle threads ready to execute another work block.
          */
         QQueue<OpenCLRunThread*> _idle;
      };
   }
}

#endif
