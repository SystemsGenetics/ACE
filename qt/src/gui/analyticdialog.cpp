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
   delete _analytic;
}






int AnalyticDialog::exec()
{
   // start timer, analytic, and call dialog exec function
   _time.start();
   _analytic->start();
   return QDialog::exec();
}






void AnalyticDialog::completeUpated(int percent)
{
   // update time remaining status label
   int elapsed {_time.elapsed()};
   int remaining = ((elapsed*100/percent)-elapsed)/1000;
   _status->setText(tr("%1 remaining.").arg(getTime(remaining)));

   // update progress bar
   _bar->setValue(percent);
}






void AnalyticDialog::analyticFinished()
{
   // set progress bar to complete
   _bar->setValue(100);

   // report total time it took in status and enable done button
   _status->setText(tr("Finished in %1.").arg(getTime(_time.elapsed()/1000)));
   _button->setDisabled(false);
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
