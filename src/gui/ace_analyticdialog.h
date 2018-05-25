#ifndef ACE_ANALYTICDIALOG_H
#define ACE_ANALYTICDIALOG_H
#include <memory>
#include <QDialog>
#include <QTime>
#include <core/ace_analytic.h>
#include "ace.h"



class QProgressBar;
class QLabel;
//



namespace Ace
{
   /*!
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
   private:
      static QString secondsToString(int seconds);
      static QString numberToString(int value);
      /*!
       */
      AnalyticThread* _thread;
      /*!
       */
      QProgressBar* _bar;
      /*!
       */
      QLabel* _info;
      /*!
       */
      QPushButton* _button;
      /*!
       */
      int _timerId;
      /*!
       */
      int _secondsLeft {0};
      /*!
       */
      int _secondsElapsed {0};
   };
}



#endif
