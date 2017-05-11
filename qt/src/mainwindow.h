#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
namespace Ace {



class MainWindow : public QMainWindow
{
   Q_OBJECT
public:
   static MainWindow& getInstance();
private slots:
   void open();
   void exit();
   void runAnalytic();
   void setOpenCL();
private:
   explicit MainWindow(QWidget *parent = nullptr);
   void createActions();
   void createMenus();
   static MainWindow* _instance;
   QAction* _openAction;
   QAction* _exitAction;
   QList<QAction*> _analyticActions;
   QAction* _setOpenCLAction;
   QMenu* _fileMenu;
   QMenu* _analyticMenu;
   QMenu* _settingsMenu;
};



}
#endif
