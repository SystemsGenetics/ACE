#ifndef ACE_ANALYTIC_RUN_H
#define ACE_ANALYTIC_RUN_H
#include <QObject>
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
          */
         virtual void start() = 0;
      signals:
         /*!
          */
         void finished();
      };
   }
}



#endif
