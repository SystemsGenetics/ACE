#ifndef ACE_ANALYTIC_SIMPLERUN_H
#define ACE_ANALYTIC_SIMPLERUN_H
#include <QObject>
#include "global.h"
//



namespace Ace
{
   namespace Analytic
   {
      /*!
       */
      class SimpleRun : public QObject
      {
         Q_OBJECT
      public:
         explicit SimpleRun(EAbstractAnalytic* analytic, QObject* parent = nullptr);
         void start();
      signals:
         /*!
          *
          * @param percentComplete  
          */
         void progressed(int percentComplete);
         /*!
          */
         void finished();
      private slots:
         void process();
      private:
         void next();
         /*!
          */
         EAbstractAnalytic* _analytic;
         /*!
          */
         int _next;
         /*!
          */
         int _percentComplete {0};
      };
   }
}



#endif
