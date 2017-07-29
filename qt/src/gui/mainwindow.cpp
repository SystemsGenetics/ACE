#include <QAction>
#include <QMenuBar>
#include <QTextEdit>
#include <QFileDialog>

#include "mainwindow.h"
#include "opencldevicedialog.h"
#include "abstractanalyticfactory.h"
#include "setupanalyticdialog.h"
#include "abstractanalytic.h"
#include "analyticdialog.h"
#include "abstractdatafactory.h"
#include "datawindow.h"
#include "datamanager.h"
#include "datareference.h"



using namespace std;
Ace::MainWindow* Ace::MainWindow::_instance {nullptr};






Ace::MainWindow& Ace::MainWindow::getInstance()
{
   // make new instance if none yet exists
   if ( !_instance )
   {
      _instance = new MainWindow;
   }

   // return reference
   return *_instance;
}






void Ace::MainWindow::addCommand(const QString &command)
{
   _console->setPlainText(_console->toPlainText().append(_commandName + " run " + command + "\n"));
}






void Ace::MainWindow::openData()
{
   // get data type from action
   QAction* from = qobject_cast<QAction*>(sender());
   quint16 type = from->data().toInt();

   // open file dialog for selecting data object file of given type
   QFileDialog dialog(nullptr,tr("Select File"));
   dialog.setAcceptMode(QFileDialog::AcceptOpen);
   EAbstractDataFactory& factory = EAbstractDataFactory::getInstance();
   QString filter = tr("%1 data object (*.%2)").arg(factory.getName(type))
         .arg(factory.getFileExtension(type));
   dialog.setNameFilter(filter);

   // run file dialog modally
   if ( dialog.exec() )
   {
      // make new data window from opened data object
      QStringList files = dialog.selectedFiles();
      DataWindow* window {new DataWindow(Ace::DataManager::getInstance().open(files.at(0)),this)};

      // set title to file name and show data window
      QFileInfo file(files.at(0));
      window->setWindowTitle(file.fileName());
      window->show();
   }
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
   // create actions and menus
   createActions();
   createMenus();

   // create central console widget
   _console = new QTextEdit(this);
   setCentralWidget(_console);
}






void Ace::MainWindow::createActions()
{
   // create data actions
   {
      EAbstractDataFactory& factory {EAbstractDataFactory::getInstance()};
      _dataActions.reserve(factory.getCount());
      for (quint16 i = 0; i < factory.getCount() ;++i)
      {
         _dataActions.append(new QAction(factory.getName(i),this));
         _dataActions.back()->setData(i);
         connect(_dataActions.back(),SIGNAL(triggered(bool)),this,SLOT(openData()));
      }
   }

   // create exit action
   _exitAction = new QAction(tr("&Exit"),this);
   _exitAction->setShortcut(QKeySequence::Close);
   _exitAction->setStatusTip(tr("Exit the application."));
   connect(_exitAction,SIGNAL(triggered(bool)),this,SLOT(close()));

   // create set opencl device action
   _setOpenCLAction = new QAction(tr("&Set OpenCL Device"),this);
   _setOpenCLAction->setStatusTip(tr("Set the active OpenCL device to a new one."));
   connect(_setOpenCLAction,SIGNAL(triggered(bool)),this,SLOT(setOpenCL()));

   // create analytic actions
   {
      EAbstractAnalyticFactory& factory {EAbstractAnalyticFactory::getInstance()};
      _analyticActions.reserve(factory.getCount());
      for (quint16 i = 0; i < factory.getCount() ;++i)
      {
         _analyticActions.append(new QAction(factory.getName(i),this));
         _analyticActions.back()->setData(i);
         connect(_analyticActions.back(),SIGNAL(triggered(bool)),this,SLOT(runAnalytic()));
      }
   }
}






void Ace::MainWindow::createMenus()
{
   // add file menu and open action
   _fileMenu = menuBar()->addMenu(tr("&File"));

   // add data submenu with all data
   _dataMenu = _fileMenu->addMenu(tr("&Open"));
   for (int i = 0; i < _dataActions.size() ;++i)
   {
      _dataMenu->addAction(_dataActions[i]);
   }

   // add analytic submenu with all analytics
   _analyticMenu = _fileMenu->addMenu(tr("&Execute"));
   for (int i = 0; i < EAbstractAnalyticFactory::getInstance().getCount() ;++i)
   {
      _analyticMenu->addAction(_analyticActions[i]);
   }

   // add exit action to file menu and settings submenu
   _fileMenu->addAction(_exitAction);
   _settingsMenu = menuBar()->addMenu(tr("&Settings"));
   _settingsMenu->addAction(_setOpenCLAction);
}
