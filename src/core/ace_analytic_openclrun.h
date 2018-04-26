#ifndef ACE_ANALYTIC_OPENCLRUN_H
#define ACE_ANALYTIC_OPENCLRUN_H
#include <memory>
#include <QVector>
#include <QQueue>
#include "ace_analytic_run.h"
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
      class OpenCLRun : public Run
      {
         Q_OBJECT
      public:
         explicit OpenCLRun(EAbstractAnalytic::OpenCL* opencl, OpenCL::Device* device, IOBase* base, QObject* parent = nullptr);
         virtual ~OpenCLRun() override final;
         void start();
      private slots:
         void blockFinished(int index);
      private:
         class Thread;
         /*!
          */
         static const int _threadSize;
         /*!
          */
         OpenCL::Context* _context;
         /*!
          */
         EAbstractAnalytic::OpenCL* _opencl;
         /*!
          */
         IOBase* _base;
         /*!
          */
         QVector<Thread*> _threads;
      };
   }
}



#endif
