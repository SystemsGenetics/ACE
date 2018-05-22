#ifndef ACE_MAINWINDOW_H
#define ACE_MAINWINDOW_H
#include <QMainWindow>



class QTextEdit;
//



namespace Ace
{
   /*!
    */
   class MainWindow : public QMainWindow
   {
      Q_OBJECT
   public:
      static MainWindow& instance();
      void addCommand(const QString& command);
   protected:
      virtual void closeEvent(QCloseEvent* event) override final;
   private slots:
      void openTriggered(quint16 type);
      void runTriggered(quint16 type);
      void settingsTriggered();
   private:
      MainWindow();
      void createActions();
      void createMenus();
      void restoreSettings();
      /*!
       */
      static const char* _stateKey;
      /*!
       */
      static const char* _geometryKey;
      /*!
       */
      static MainWindow* _instance;
      /*!
       */
      QList<QAction*> _openActions;
      /*!
       */
      QList<QAction*> _runActions;
      /*!
       */
      QAction* _exitAction;
      /*!
       */
      QAction* _settingsAction;
      /*!
       */
      QTextEdit* _console;
   };
}



#endif
