#ifndef SETUPANALYTICDIALOG_H
#define SETUPANALYTICDIALOG_H
#include <QDialog>



class QLineEdit;
class QGridLayout;
class AbstractAnalytic;



class SetupAnalyticDialog : public QDialog
{
public:
   SetupAnalyticDialog(AbstractAnalytic* analytic, QWidget* parent = nullptr);
   ~SetupAnalyticDialog();
private slots:
   void findFile(int argument);
   void cancel();
   void executeButton();
private:
   QGridLayout* createInputs();
   QList<QWidget*> _inputs;
   AbstractAnalytic* _analytic;
};



#endif
