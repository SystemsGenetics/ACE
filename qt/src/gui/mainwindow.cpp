#include <QAction>
#include <QMenuBar>
#include <QTextEdit>
#include <iostream>

#include "mainwindow.h"
#include "opencldevicedialog.h"
#include "abstractanalyticfactory.h"
#include "setupanalyticdialog.h"
#include "abstractanalytic.h"
#include "analyticdialog.h"



using namespace std;
Ace::MainWindow* Ace::MainWindow::_instance {nullptr};






Ace::MainWindow& Ace::MainWindow::getInstance()
{
   if ( !_instance )
   {
      _instance = new MainWindow;
   }
   return *_instance;
}






void Ace::MainWindow::addCommand(const QString &command)
{
   _console->setPlainText(_console->toPlainText().append(command + "\n"));
}






void Ace::MainWindow::open()
{

}






void Ace::MainWindow::exit()
{

}






void Ace::MainWindow::runAnalytic()
{
   // get analytic type from action and create new analytic of that type
   QAction* from = qobject_cast<QAction*>(sender());
   EAbstractAnalyticFactory& factory = EAbstractAnalyticFactory::getInstance();
   quint16 type = from->data().toInt();
   unique_ptr<EAbstractAnalytic> analytic(factory.make(type));

   // create setup analytic dialog and run it
   SetupAnalyticDialog dialog(analytic.get());
   dialog.setWindowTitle(tr("Execute %1").arg(factory.getName(type)));
   if ( dialog.exec() )
   {
      // if setup analytic dialog is successful run analytic through analytic dialog
      AnalyticDialog runDialog(analytic.release());
      runDialog.setWindowTitle(tr("Executing %1").arg(factory.getName(type)));
      runDialog.exec();
   }
}






void Ace::MainWindow::setOpenCL()
{
   OpenCLDeviceDialog::getInstance().exec();
}






Ace::MainWindow::MainWindow(QWidget *parent):
   QMainWindow(parent)
{
   createActions();
   createMenus();
   _console = new QTextEdit(this);
   setCentralWidget(_console);
}






void Ace::MainWindow::createActions()
{
   // create open action
   _openAction = new QAction(tr("&Open Data"),this);
   _openAction->setShortcut(QKeySequence::Open);
   _openAction->setStatusTip(tr("Open an existing data object file."));
   connect(_openAction,SIGNAL(triggered(bool)),this,SLOT(open()));

   // create exit action
   _exitAction = new QAction(tr("&Exit"),this);
   _exitAction->setShortcut(QKeySequence::Close);
   _exitAction->setStatusTip(tr("Exit the application."));
   connect(_exitAction,SIGNAL(triggered(bool)),this,SLOT(exit()));

   // create set opencl device action
   _setOpenCLAction = new QAction(tr("&Set OpenCL Device"),this);
   _setOpenCLAction->setStatusTip(tr("Set the active OpenCL device to a new one."));
   connect(_setOpenCLAction,SIGNAL(triggered(bool)),this,SLOT(setOpenCL()));

   // create analytic actions
   _analyticActions.reserve(EAbstractAnalyticFactory::getInstance().getCount());
   for (quint16 i = 0; i < EAbstractAnalyticFactory::getInstance().getCount() ;++i)
   {
      _analyticActions.append(new QAction(EAbstractAnalyticFactory::getInstance().getName(i),this));
      _analyticActions.back()->setData(QVariant(i));
      connect(_analyticActions.back(),SIGNAL(triggered(bool)),this,SLOT(runAnalytic()));
   }
}






void Ace::MainWindow::createMenus()
{
   // add file menu and open action
   _fileMenu = menuBar()->addMenu(tr("&File"));
   _fileMenu->addAction(_openAction);

   // add analytic submenu with all analytics
   _analyticMenu = _fileMenu->addMenu(tr("&Analytics"));
   for (int i = 0; i < EAbstractAnalyticFactory::getInstance().getCount() ;++i)
   {
      _analyticMenu->addAction(_analyticActions[i]);
   }

   // add exit action to file menu and settings submenu
   _fileMenu->addAction(_exitAction);
   _settingsMenu = menuBar()->addMenu(tr("&Settings"));
   _settingsMenu->addAction(_setOpenCLAction);
}
