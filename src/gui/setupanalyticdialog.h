#ifndef ACE_SETUPANALYTICDIALOG_H
#define ACE_SETUPANALYTICDIALOG_H
#include <QDialog>



class QLineEdit;
class QFormLayout;
class EAbstractAnalytic;



namespace Ace
{
   class SetupAnalyticDialog : public QDialog
   {
      Q_OBJECT
   public:
      SetupAnalyticDialog(EAbstractAnalytic* analytic, QString commandName
                          , QWidget* parent = nullptr);
   private slots:
      void findFile(int argument);
      void cancel();
      void executeButton();
   private:
      QFormLayout* createInputs();
      QList<QWidget*> _inputs;
      EAbstractAnalytic* _analytic;
      QPushButton* _cancelButton;
      QPushButton* _executeButton;
      QString _commandName;
   };
}



#endif
