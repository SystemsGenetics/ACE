#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>



class MainWindow : public QMainWindow
{
   Q_OBJECT
public:
   explicit MainWindow(QWidget *parent = nullptr);
private slots:
   void open();
   void close();
   void runAnalytic();
   void setOpenCL();
private:
   void createActions();
   void createMenus();
   QAction* _openAction;
   QAction* _closeAction;
   QList<QAction*> _analyticActions;
   QAction* _setOpenCLAction;
   QMenu* _fileMenu;
   QMenu* _analyticMenu;
   QMenu* _settingsMenu;
};



#endif
