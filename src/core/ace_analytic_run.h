#ifndef ACE_ANALYTIC_RUN_H
#define ACE_ANALYTIC_RUN_H
#include <memory>
#include <QObject>
#include "eabstractanalytic.h"
//



namespace Ace
{
   namespace Analytic
   {
      /*!
       */
      class Run : public QObject
      {
         Q_OBJECT
      public:
         explicit Run(QObject* parent = nullptr);
         /*!
          *
          * @param block  
          */
         virtual void addWork(std::unique_ptr<EAbstractAnalytic::Block>&& block) = 0;
      signals:
         /*!
          */
         void finished();
      };
   }
}



#endif
