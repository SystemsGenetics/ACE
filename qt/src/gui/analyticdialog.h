#ifndef ANALYTICDIALOG_H
#define ANALYTICDIALOG_H
#include <QDialog>
#include <QTime>



class EAbstractAnalytic;
class QProgressBar;
class QLabel;



namespace Ace
{
class AnalyticDialog : public QDialog
{
   Q_OBJECT
public:
   AnalyticDialog(EAbstractAnalytic* analytic, QWidget* parent = nullptr);
   ~AnalyticDialog();
   int exec() override final;
private slots:
   void completeUpated(int percent);
   void analyticFinished();
   void exceptionThrown(QString file, int line, QString function, QString title, QString details);
private:
   QString getTime(int seconds);
   int getSeconds(int total);
   int getMinutes(int total);
   int getHours(int total);
   int getDays(int total);
   QString getTimeUnit(int amount, const QString& unit);
   EAbstractAnalytic* _analytic;
   QProgressBar* _bar;
   QPushButton* _button;
   QTime _time;
   QLabel* _status;
};
}



#endif
