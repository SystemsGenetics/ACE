#ifndef ACE_ANALYTICDIALOG_H
#define ACE_ANALYTICDIALOG_H
#include <memory>
#include <QDialog>
#include <QTime>
#include "../core/ace_analytic.h"
#include "ace.h"



class QProgressBar;
class QLabel;
//



namespace Ace
{
   /*!
    * This is the analytic dialog. This dialog handles actual execution of an analytic 
    * manager. The manager is moved to a separate analytic thread for execution so the 
    * main GUI thread can remain responsive. This dialog presents the percent complete 
    * along with an estimated time remaining status to the user while execution is 
    * happening. When execution finishes a done button is enabled to close the window 
    * along with showing how much time execution took. Execution of the analytic 
    * manager for this dialog begins when the dialog is executed in modal. 
    */
   class AnalyticDialog : public QDialog
   {
      Q_OBJECT
   public:
      AnalyticDialog(std::unique_ptr<Analytic::AbstractManager>&& manager);
      virtual int exec() override final;
   protected:
      virtual void closeEvent(QCloseEvent* event) override final;
      virtual void timerEvent(QTimerEvent* event) override final;
   private slots:
      void progressed(int percentComplete);
      void done();
      void finished();
   private:
      static QString secondsToString(int seconds);
      static QString numberToString(int value);
      /*!
       * The analytic thread where this object's analytic manager is executed. 
       */
      AnalyticThread* _thread;
      /*!
       * The progress bar for this dialog that displays the percent complete for its 
       * executing analytic manager. 
       */
      QProgressBar* _bar;
      /*!
       * The info bar that displays the estimated time remaining if execution is in 
       * progress or the total time taken if execution is finished for this dialog. 
       */
      QLabel* _info;
      /*!
       * The done button for this dialog that is enabled when execution is finished. 
       */
      QPushButton* _button;
      /*!
       * The timer ID of the timer used to count the number of seconds since analytic 
       * execution began. 
       */
      int _timerId;
      /*!
       * The estimated number of seconds left for this object's analytic execution to 
       * finish. 
       */
      int _secondsLeft {0};
      /*!
       * The total number of seconds elapsed since this dialog began its analytic 
       * execution. 
       */
      int _secondsElapsed {0};
   };
}



#endif
