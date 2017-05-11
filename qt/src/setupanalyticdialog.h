#ifndef ACE_SETUPANALYTICDIALOG_H
#define ACE_SETUPANALYTICDIALOG_H
#include <QDialog>



class QLineEdit;
class QGridLayout;
class EAbstractAnalytic;



namespace Ace
{
   class SetupAnalyticDialog : public QDialog
   {
   public:
      SetupAnalyticDialog(EAbstractAnalytic* analytic, QWidget* parent = nullptr);
      ~SetupAnalyticDialog();
   private slots:
      void findFile(int argument);
      void cancel();
      void executeButton();
   private:
      QGridLayout* createInputs();
      QList<QWidget*> _inputs;
      EAbstractAnalytic* _analytic;
   };
}



#endif
