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
    * This is the analytic thread used to run the execution of an analytic manager. 
    * The only functionality this adds is allowing an exception to be set to it if the 
    * main event loop catches one outside of the GUI thread and the thread happens to 
    * be an analytic thread. 
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
       * Pointer to the analytic manager that is executed in this thread. 
       */
      Analytic::AbstractManager* _manager;
      /*!
       * Pointer to an exception that occurred within this thread or null if no exception 
       * has occurred. 
       */
      EException* _exception {nullptr};
   };
}



#endif
