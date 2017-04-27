#include <QAction>
#include <QMenuBar>
#include <QTextEdit>

#include "mainwindow.h"
#include "opencldevicedialog.h"
#include "abstractanalyticfactory.h"






MainWindow::MainWindow(QWidget *parent):
   QMainWindow(parent)
{
   createActions();
   createMenus();
   setCentralWidget(new QTextEdit(this));
}






void MainWindow::open()
{

}






void MainWindow::close()
{

}






void MainWindow::runAnalytic()
{

}






void MainWindow::setOpenCL()
{
   OpenCLDeviceDialog::getInstance().exec();
}






void MainWindow::createActions()
{
   // create open action
   _openAction = new QAction(tr("&Open"),this);
   _openAction->setShortcut(QKeySequence::Open);
   _openAction->setStatusTip(tr("Open an existing data object file."));
   connect(_openAction,&QAction::triggered,this,&MainWindow::open);

   // create close action
   _closeAction = new QAction(tr("&Close"),this);
   _closeAction->setShortcut(QKeySequence::Close);
   _closeAction->setStatusTip(tr("Close the application."));
   connect(_closeAction,&QAction::triggered,this,&MainWindow::close);

   // create set opencl device action
   _setOpenCLAction = new QAction(tr("&Set OpenCL Device"),this);
   _setOpenCLAction->setStatusTip(tr("Set the active OpenCL device to a new one."));
   connect(_setOpenCLAction,&QAction::triggered,this,&MainWindow::setOpenCL);

   // create analytic actions
   _analyticActions.reserve(AbstractAnalyticFactory::getInstance().getCount());
   for (int i = 0; i < AbstractAnalyticFactory::getInstance().getCount() ;++i)
   {
      _analyticActions.append(new QAction(AbstractAnalyticFactory::getInstance().getName(i),this));
      _analyticActions.back()->setData(QVariant(i));
      connect(_analyticActions.back(),&QAction::triggered,this,&MainWindow::runAnalytic);
   }
}






void MainWindow::createMenus()
{
   // create file menu
   _fileMenu = menuBar()->addMenu(tr("&File"));
   _fileMenu->addAction(_openAction);
   _fileMenu->addAction(_closeAction);

   // create analytic menu
   _analyticMenu = menuBar()->addMenu(tr("&Analytics"));
   for (int i = 0; i < AbstractAnalyticFactory::getInstance().getCount() ;++i)
   {
      _analyticMenu->addAction(_analyticActions[i]);
   }

   // create settings menu
   _settingsMenu = menuBar()->addMenu(tr("&Settings"));
   _settingsMenu->addAction(_setOpenCLAction);
}
