#include <QProgressBar>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "analyticdialog.h"
#include "abstractanalytic.h"



using namespace Ace;






AnalyticDialog::AnalyticDialog(EAbstractAnalytic *analytic, QWidget *parent):
   QDialog(parent),
   _analytic(analytic)
{
   // create progress bar
   _bar = new QProgressBar;
   _bar->setMinimum(0);
   _bar->setMaximum(100);
   _bar->setValue(0);

   // create status and button widgets
   _status = new QLabel(tr("Remaining Time Unknown."));
   _button = new QPushButton(tr("&Done"));
   _button->setDisabled(true);

   // connect all signals
   connect(_analytic,SIGNAL(progressed(int)),this,SLOT(completeUpated(int)));
   connect(_analytic,SIGNAL(finished()),this,SLOT(analyticFinished()));
   connect(_analytic,SIGNAL(exceptionThrown(QString,int,QString,QString,QString)),this
           ,SLOT(exceptionThrown(QString,int,QString,QString,QString)));
   connect(_button,SIGNAL(clicked(bool)),this,SLOT(accept()));

   // create main layout for dialog and set it to dialog's layout
   QVBoxLayout* layout = new QVBoxLayout;
   layout->addWidget(_bar);
   layout->addWidget(_status);
   layout->addWidget(_button);
   setLayout(layout);
}






AnalyticDialog::~AnalyticDialog()
{
   _analytic->stop();
   delete _analytic;
}






int AnalyticDialog::exec()
{
   // start timers, analytic, and call dialog exec function
   _id = startTimer(1000);
   _time.start();
   _analytic->start();
   return QDialog::exec();
}






void AnalyticDialog::completeUpated(int percent)
{
   // update time remaining status label
   int elapsed {_time.elapsed()};
   _secondsLeft = ((elapsed*100/percent)-elapsed)/1000;

   // update progress bar
   _bar->setValue(percent);
}






void AnalyticDialog::analyticFinished()
{
   // set progress bar to complete and kill timer
   _bar->setValue(100);
   killTimer(_id);

   // report total time it took in status and enable done button
   _status->setText(tr("Finished in %1.").arg(getTime(_time.elapsed()/1000)));
   _button->setDisabled(false);
}






void AnalyticDialog::exceptionThrown(QString file, int line, QString function, QString title
                                     , QString details)
{
   // reconstitute exception that was thrown in analytic thread
   EException e;
   e.setFile(file);
   e.setLine(line);
   e.setFunction(function);
   e.setTitle(title);
   e.setDetails(details);

   // throw exception in main gui thread
   throw e;
}






void AnalyticDialog::timerEvent(QTimerEvent* event)
{
   // only one timer is used so ignore event
   Q_UNUSED(event);

   // if seconds is less than zero say time unknown
   if ( _secondsLeft < 0 )
   {
      _status->setText(tr("Remaining Time Unknown."));
   }

   // else decrement seconds left and report estimated time left
   else
   {
      if ( _secondsLeft > 0 )
      {
         --_secondsLeft;
      }
      _status->setText(tr("%1 remaining.").arg(getTime(_secondsLeft)));
   }
}






QString AnalyticDialog::getTime(int seconds)
{
   // determine if there are any seconds
   if ( seconds > 0 )
   {
      // create string reporting tiem units left and return it
      QString status;
      status.append(getTimeUnit(getDays(seconds),tr("day")));
      status.append(getTimeUnit(getHours(seconds),tr("hour")));
      status.append(getTimeUnit(getMinutes(seconds),tr("minute")));
      status.append(getTimeUnit(getSeconds(seconds),tr("second")));
      status.remove(status.size()-1,1);
      return status;
   }
   else
   {
      // return less than a second string
      return tr("Less than a second");
   }
}






int AnalyticDialog::getSeconds(int total)
{
   return total%60;
}






int AnalyticDialog::getMinutes(int total)
{
   // return minutes
   total /= 60;
   return total%60;
}






int AnalyticDialog::getHours(int total)
{
   // return hours
   total /= 3600;
   return total%24;
}






int AnalyticDialog::getDays(int total)
{
   return total/86400;
}






QString AnalyticDialog::getTimeUnit(int amount, const QString& unit)
{
   if ( amount > 1 )
   {
      // if amount is greater than one return plural unit
      return QString("%1 %2s ").arg(amount).arg(unit);
   }
   else if ( amount == 1 )
   {
      // if amount is one return singular
      return QString("1 %1 ").arg(unit);
   }

   // if amount is less than one return nothing
   return QString();
}
