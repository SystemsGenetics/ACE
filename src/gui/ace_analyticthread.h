#ifndef ACE_ANALYTICTHREAD_H
#define ACE_ANALYTICTHREAD_H
#include <memory>
#include <QThread>
#include <../core/ace_analytic.h>
#include <../core/global.h>
//



namespace Ace
{
   /*!
    */
   class AnalyticThread : public QThread
   {
      Q_OBJECT
   public:
      AnalyticThread(Analytic::AbstractManager* manager);
      void check();
      void setException(const EException& exception);
   protected:
      virtual void run() override final;
   private:
      /*!
       */
      Analytic::AbstractManager* _manager;
      /*!
       */
      EException* _exception {nullptr};
   };
}



#endif
