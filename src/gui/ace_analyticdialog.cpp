#include "ace_analyticdialog.h"
#include <QProgressBar>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QCloseEvent>
#include <../core/ace_analytic_abstractmanager.h>
#include "ace_analyticthread.h"



using namespace Ace;
//






/*!
 *
 * @param manager  
 */
AnalyticDialog::AnalyticDialog(std::unique_ptr<Analytic::AbstractManager>&& manager)
{
   _bar = new QProgressBar;
   _bar->setMinimum(0);
   _bar->setMaximum(100);
   _bar->setValue(0);

   _info = new QLabel(tr("Remaining Time Unknown."));
   _button = new QPushButton(tr("&Done"));
   _button->setDisabled(true);

   connect(manager.get()
           ,&Analytic::AbstractManager::progressed
           ,this
           ,&AnalyticDialog::progressed
           ,Qt::QueuedConnection);
   connect(manager.get()
           ,&Analytic::AbstractManager::done
           ,this
           ,&AnalyticDialog::done
           ,Qt::QueuedConnection);

   _thread = new AnalyticThread(std::move(manager),this);
   connect(_thread,&AnalyticThread::finished,this,&QDialog::accept);

   QVBoxLayout* layout = new QVBoxLayout;
   layout->addWidget(_bar);
   layout->addWidget(_info);
   layout->addWidget(_button);
   setLayout(layout);
}






/*!
 */
int AnalyticDialog::exec()
{
   _timerId = startTimer(1000);
   _thread->start();
   return QDialog::exec();
}






/*!
 *
 * @param event  
 */
void AnalyticDialog::closeEvent(QCloseEvent* event)
{
   event->ignore();
}






/*!
 *
 * @param event  
 */
void AnalyticDialog::timerEvent(QTimerEvent* event)
{
   Q_UNUSED(event);
   _info->setText(secondsToString(_secondsLeft--).append(" remaining (D:HH:MM:SS)"));
   if ( _secondsLeft < 0 )
   {
      _secondsLeft = 0;
   }
   ++_secondsElapsed;
}






/*!
 *
 * @param percentComplete  
 */
void AnalyticDialog::progressed(int percentComplete)
{
   _bar->setValue(percentComplete);
   if ( percentComplete > 0 )
   {
      _secondsLeft = (_secondsElapsed*100/percentComplete) - _secondsElapsed;
   }
}






/*!
 */
void AnalyticDialog::done()
{
   _bar->setValue(100);
   killTimer(_timerId);
   _info->setText(tr("Finished in %1 (D:HH:MM:SS)").arg(secondsToString(_secondsElapsed)));
   _button->setDisabled(false);
}






/*!
 *
 * @param seconds  
 */
QString AnalyticDialog::secondsToString(int seconds)
{
   int days {seconds};
   seconds = days%60;
   days /= 60;
   int minutes {days%60};
   days /= 60;
   int hours {days%24};
   days /= 24;
   return QString::number(days).append(":")
         .append(numberToString(hours))
         .append(":")
         .append(numberToString(minutes))
         .append(":")
         .append(numberToString(seconds));
}






/*!
 *
 * @param value  
 */
QString AnalyticDialog::numberToString(int value)
{
   QString ret;
   if ( value < 0 || value > 60 )
   {
      return ret;
   }
   if ( value < 10 )
   {
      ret.append("0");
   }
   return ret.append(QString::number(value));
}
