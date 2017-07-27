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
   _bar = new QProgressBar;
   _bar->setMinimum(0);
   _bar->setMaximum(100);
   _bar->setValue(0);
   _status = new QLabel(tr("Remaining Time Unknown."));
   _button = new QPushButton(tr("&Done"));
   _button->setDisabled(true);
   connect(_analytic,SIGNAL(progressed(int)),this,SLOT(completeUpated(int)));
   connect(_analytic,SIGNAL(finished()),this,SLOT(analyticFinished()));
   connect(_button,SIGNAL(clicked(bool)),this,SLOT(accept()));
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
   _time.start();
   _analytic->start();
   return QDialog::exec();
}






void AnalyticDialog::completeUpated(int percent)
{
   int elapsed {_time.elapsed()};
   int remaining = ((elapsed*100/percent)-elapsed)/1000;
   _status->setText(tr("%1 remaining.").arg(getTime(remaining)));
   _bar->setValue(percent);
}






void AnalyticDialog::analyticFinished()
{
   _bar->setValue(100);
   _status->setText(tr("Finished in %1.").arg(getTime(_time.elapsed()/1000)));
   _button->setDisabled(false);
}






QString AnalyticDialog::getTime(int seconds)
{
   if ( seconds > 0 )
   {
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
      return tr("Less than a second");
   }
}






int AnalyticDialog::getSeconds(int total)
{
   return total%60;
}






int AnalyticDialog::getMinutes(int total)
{
   total /= 60;
   return total%60;
}






int AnalyticDialog::getHours(int total)
{
   total /= 3600;
   return total%24;
}






int AnalyticDialog::getDays(int total)
{
   total /= 86400;
   return total;
}






QString AnalyticDialog::getTimeUnit(int amount, const QString& unit)
{
   if ( amount > 1 )
   {
      return QString("%1 %2s ").arg(amount).arg(unit);
   }
   else if ( amount == 1 )
   {
      return QString("1 %1 ").arg(unit);
   }
   return QString();
}
