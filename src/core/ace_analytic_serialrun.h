#ifndef ACE_ANALYTIC_SERIALRUN_H
#define ACE_ANALYTIC_SERIALRUN_H
#include <QObject>
#include "ace_analytic.h"
#include "eabstractanalytic.h"
//



namespace Ace
{
   namespace Analytic
   {
      /*!
       */
      class SerialRun : public QObject
      {
         Q_OBJECT
      public:
         explicit SerialRun(EAbstractAnalytic* analytic, EAbstractAnalytic::Serial* serial, IOBase* base, QObject* parent = nullptr);
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
         EAbstractAnalytic::Serial* _serial;
         /*!
          */
         IOBase* _base;
         /*!
          */
         int _next {0};
         /*!
          */
         int _percentComplete {0};
      };
   }
}



#endif
