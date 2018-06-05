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
      AnalyticThread(std::unique_ptr<Analytic::AbstractManager>&& manager, QObject* parent = nullptr);
      void check();
   protected:
      virtual void run() override final;
   private slots:
      void managerFinished();
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
