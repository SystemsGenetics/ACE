#include "ace_analyticdialog.h"
#include <QProgressBar>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QCloseEvent>
#include "../core/ace_analytic_abstractmanager.h"
#include "ace_analyticthread.h"



using namespace Ace;
//






/*!
 * Constructs a new analytic dialog with the given analytic manager. 
 *
 * @param manager The analytic manager that this new analytic dialog will start 
 *                executing when executed in modal. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create and initialize this new object's progress bar. 
 *
 * 2. Create and initialize this new object's info bar, then create the done button 
 *    disabling it and connecting its clicked signal to close this dialog on 
 *    success. 
 *
 * 3. Create this new object's analytic thread, then move the given analytic 
 *    manager to the new analytic thread, then connect all required signals for 
 *    execution of the analytic to start when its thread starts, and then release 
 *    the manager's smart pointer. 
 *
 * 4. Create a new vertical layout, adding this new object's progress bar then its 
 *    info bar and then its done button. Set the layout of this dialog to the new 
 *    vertical layout. 
 */
AnalyticDialog::AnalyticDialog(std::unique_ptr<Analytic::AbstractManager>&& manager)
{
   // Step 1
   _bar = new QProgressBar;
   _bar->setMinimum(0);
   _bar->setMaximum(100);
   _bar->setValue(0);

   // Step 2
   _info = new QLabel(tr("Remaining Time Unknown."));
   _button = new QPushButton(tr("&Done"));
   _button->setDisabled(true);
   connect(_button,&QPushButton::clicked,this,&QDialog::close);

   // Step 3
   _thread = new AnalyticThread(manager.get());
   manager->moveToThread(_thread);
   connect(_thread,&QThread::started,manager.get(),&Analytic::AbstractManager::initialize);
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
   connect(_thread,&QThread::finished,this,&AnalyticDialog::finished,Qt::QueuedConnection);
   connect(manager.get(),&Analytic::AbstractManager::finished,manager.get(),&QObject::deleteLater);
   manager.release();

   // Step 4
   QVBoxLayout* layout = new QVBoxLayout;
   layout->addWidget(_bar);
   layout->addWidget(_info);
   layout->addWidget(_button);
   setLayout(layout);
}






/*!
 * Implements the qt interface that begins modal execution of this dialog. This 
 * implementation starts this object's seconds timer, its thread, and then calls 
 * the qt dialog interface. 
 *
 * @return The return status returned by the qt dialog. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Start this object's seconds timer, then its analytic thread, and then call 
 *    the qt dialog interface returning its return. 
 */
int AnalyticDialog::exec()
{
   // Step 1
   _timerId = startTimer(1000);
   _thread->start();
   return QDialog::exec();
}






/*!
 * Implements the qt interface that is called when this dialog has a close event. 
 * This implementation prevents this dialog from being closed if its analytic 
 * thread is running. 
 *
 * @param event Pointer to the qt close event being processed. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this object's analytic thread is running then reject the event, else 
 *    accept the event. 
 */
void AnalyticDialog::closeEvent(QCloseEvent* event)
{
   // Step 1
   if ( _thread && _thread->isRunning() )
   {
      event->ignore();
   }
   else
   {
      event->accept();
   }
}






/*!
 * Implements the qt interface that is called whenever a qt timer for this object 
 * fires. This class only has one timer for counting the seconds since start of 
 * execution. 
 *
 * @param event  
 *
 *
 * Steps of Operation: 
 *
 * 1. Update the info bar that predicts the total time remaining for execution, 
 *    reducing this object's estimated seconds left by one. If the seconds left is 
 *    less than zero then set it back to zero. 
 *
 * 2. Increment this object's seconds elapsed by one. 
 */
void AnalyticDialog::timerEvent(QTimerEvent* event)
{
   Q_UNUSED(event);

   // Step 1
   _info->setText(secondsToString(_secondsLeft--).append(" remaining (D:HH:MM:SS)"));
   if ( _secondsLeft < 0 )
   {
      _secondsLeft = 0;
   }

   // Step 2
   ++_secondsElapsed;
}






/*!
 * Called when the analytic manager being executed by this dialog has made progress 
 * in its completion by the given percentage. 
 *
 * @param percentComplete The percent complete out of 100. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Update this object's progress bar. If the given percent complete is greater 
 *    than zero then recalculate this object's estimated seconds left. 
 */
void AnalyticDialog::progressed(int percentComplete)
{
   // Step 1
   _bar->setValue(percentComplete);
   if ( percentComplete > 0 )
   {
      _secondsLeft = (_secondsElapsed*100/percentComplete) - _secondsElapsed;
   }
}






/*!
 * Called when the analytic manager being executed by this dialog is done. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Set this object's progress bar to 100 percent done, kill its timer, set its 
 *    info bar to total time taken, and enable its done button. 
 */
void AnalyticDialog::done()
{
   // Step 1
   _bar->setValue(100);
   killTimer(_timerId);
   _info->setText(tr("Finished in %1 (D:HH:MM:SS)").arg(secondsToString(_secondsElapsed)));
   _button->setDisabled(false);
}






/*!
 * Called when the analytic manager being executed is completely finished and ready 
 * to be deleted. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Call on the analytic thread of this dialog to be deleted later and then check 
 *    it for any exception it may contain. If the check method throws an exception 
 *    then call on this dialog to close execution in failure and throw the 
 *    exception again. 
 */
void AnalyticDialog::finished()
{
   // Step 1
   try
   {
      AnalyticThread* thread {_thread};
      _thread = nullptr;
      thread->deleteLater();
      thread->check();
   }
   catch (...)
   {
      reject();
      close();
      throw;
   }
}






/*!
 * Convert and return the given number of seconds into a string in the format of 
 * hours, minutes, and seconds. 
 *
 * @param seconds The number of seconds that is converted into a string. 
 *
 * @return The given number of seconds as a string in the format of hours, minutes, 
 *         and seconds. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Determine the number of hours, minutes and seconds from the given total 
 *    number of seconds. 
 *
 * 2. Return a string containing the hours, minutes, and seconds computed in the 
 *    format "HH:MM:SS". 
 */
QString AnalyticDialog::secondsToString(int seconds)
{
   // Step 1
   int days {seconds};
   seconds = days%60;
   days /= 60;
   int minutes {days%60};
   days /= 60;
   int hours {days%24};
   days /= 24;

   // Step 2
   return QString::number(days).append(":")
         .append(numberToString(hours))
         .append(":")
         .append(numberToString(minutes))
         .append(":")
         .append(numberToString(seconds));
}






/*!
 * Converts the given number to a string that always has two digits, with a leading 
 * 0 if needed. If the given number is not between 0 and 60 then an exception is 
 * thrown. 
 *
 * @param value The number that is converted to a string with two digits. 
 *
 * @return The given number as a string that will always contain two digits. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Create an empty string _ret_. If the given value if out of range then return 
 *    _ret_ else if the given value is less than ten then append a 0 to _ret_. 
 *
 * 2. Append the given number to _ret_ and then return it. 
 */
QString AnalyticDialog::numberToString(int value)
{
   // Step 1
   QString ret;
   if ( value < 0 || value > 60 )
   {
      return ret;
   }
   else if ( value < 10 )
   {
      ret.append("0");
   }

   // Step 2
   return ret.append(QString::number(value));
}
