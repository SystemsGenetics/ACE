#ifndef ACE_MAINWINDOW_H
#define ACE_MAINWINDOW_H
#include <QMainWindow>



class QTextEdit;
//



namespace Ace
{
   /*!
    * This is the main window for an ACE program. This is a singleton class that 
    * allows only one global instance since this is the main window. It is also done 
    * this way because analytic dialog windows need to access this main window for 
    * adding example commands. As mentioned this main window provides an interface for 
    * adding example commands to its central text edit widget. 
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
       * The key used to save the state of this window in qt settings. 
       */
      static const char* _stateKey;
      /*!
       * The key used to save the geometry of this window in qt settings. 
       */
      static const char* _geometryKey;
      /*!
       * Pointer to the global instance of this main window. 
       */
      static MainWindow* _instance;
      /*!
       * List of all open actions corresponding to all available data object types for 
       * this main window. 
       */
      QList<QAction*> _openActions;
      /*!
       * List of all run actions corresponding to all available analytic types for this 
       * main window. 
       */
      QList<QAction*> _runActions;
      /*!
       * The exit action for this main window. 
       */
      QAction* _exitAction;
      /*!
       * The settings action for this main window. 
       */
      QAction* _settingsAction;
      /*!
       * The central text edit widget for this main window that displays a list of any 
       * example command line version of executed analytic runs. 
       */
      QTextEdit* _console;
   };
}



#endif
