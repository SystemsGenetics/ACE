#ifndef ACE_ANALYTIC_ABSTRACTRUN_H
#define ACE_ANALYTIC_ABSTRACTRUN_H
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
      class AbstractRun : public QObject
      {
         Q_OBJECT
      public:
         explicit AbstractRun(QObject* parent = nullptr);
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
