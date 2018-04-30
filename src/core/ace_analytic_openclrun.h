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
          */
         OpenCL::Context* _context;
         /*!
          */
         EAbstractAnalytic::OpenCL* _opencl;
         /*!
          */
         AbstractInput* _base;
         /*!
          */
         QVector<Thread*> _threads;
         /*!
          */
         QQueue<Thread*> _idle;
      };
   }
}



#endif
