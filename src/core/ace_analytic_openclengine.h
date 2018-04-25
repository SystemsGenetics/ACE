#ifndef ACE_ANALYTIC_OPENCLENGINE_H
#define ACE_ANALYTIC_OPENCLENGINE_H
#include <memory>
#include <QObject>
#include <QQueue>
#include "opencl.h"
#include "eabstractanalytic.h"
//



namespace Ace
{
   namespace Analytic
   {
      /*!
       */
      class OpenCLEngine : public QObject
      {
         Q_OBJECT
      public:
         explicit OpenCLEngine(std::unique_ptr<EAbstractAnalytic::OpenCL>&& opencl, OpenCL::Device* device, QObject* parent = nullptr);
         bool isFull() const;
         bool hasResults() const;
         void execute(std::unique_ptr<EAbstractAnalytic::Block>&& block);
         std::unique_ptr<EAbstractAnalytic::Block> result();
      signals:
         /*!
          */
         void finished();
      private slots:
         void blockFinished(int index);
      private:
         class Piston;
         /*!
          */
         static const int _pistonSize;
         /*!
          */
         OpenCL::Context* _context;
         /*!
          */
         EAbstractAnalytic::OpenCL* _opencl;
         /*!
          */
         QList<Piston*> _pistons;
         /*!
          */
         QQueue<Piston*> _idle;
         /*!
          */
         QQueue<EAbstractAnalytic::Block*> _results;
      };
   }
}



#endif
