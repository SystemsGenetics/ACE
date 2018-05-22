#include "ace_mainwindow.h"
#include <QAction>
#include <QMenuBar>
#include <QTextEdit>
#include <QFileDialog>
#include <QCloseEvent>
#include <QSettings>
#include <core/eabstractdatafactory.h>
#include <core/eabstractanalyticfactory.h>
#include <core/ace_dataobject.h>
#include <core/ace_settings.h>
#include "ace_datawindow.h"
#include "opencldevicedialog.h"
#include "setupanalyticdialog.h"
#include "analyticdialog.h"



using namespace std;
using namespace Ace;
//



/*!
 */
const char* MainWindow::_stateKey {"ace.mainwindow.state"};
/*!
 */
const char* MainWindow::_geometryKey {"ace.mainwindow.geometry"};
/*!
 */
MainWindow* MainWindow::_instance {nullptr};






/*!
 */
MainWindow& MainWindow::instance()
{
   if ( !_instance )
   {
      _instance = new MainWindow;
   }
   return *_instance;
}






/*!
 *
 * @param command  
 */
void MainWindow::addCommand(const QString& command)
{
   QString line
   {
      Ace::Settings::instance().application().append(" run ").append(command).append("\n")
   };
   _console->setPlainText(_console->toPlainText().append(line));
}






/*!
 *
 * @param event  
 */
void MainWindow::closeEvent(QCloseEvent* event)
{
   QSettings settings(Ace::Settings::instance().organization()
                      ,Ace::Settings::instance().application());
   settings.setValue(_stateKey,saveState());
   settings.setValue(_geometryKey,saveGeometry());
   event->accept();
}






/*!
 *
 * @param type  
 */
void MainWindow::openTriggered(quint16 type)
{
   QFileDialog dialog(this,tr("Select File"));
   dialog.setAcceptMode(QFileDialog::AcceptOpen);
   EAbstractDataFactory& factory = EAbstractDataFactory::instance();
   dialog.setNameFilter(tr("%1 data object (*.%2)")
                        .arg(factory.name(type))
                        .arg(factory.fileExtension(type)));
   if ( dialog.exec() )
   {
      QStringList files = dialog.selectedFiles();
      DataWindow* window
      {
         new DataWindow(unique_ptr<DataObject>(new DataObject(files.first())),this)
      };
      QFileInfo file(files.at(0));
      window->setWindowTitle(QString("%1 (%2)").arg(file.fileName()).arg(factory.name(type)));
      window->show();
   }
}






/*!
 *
 * @param type  
 */
void MainWindow::runTriggered(quint16 type)
{
   EAbstractAnalyticFactory& factory {EAbstractAnalyticFactory::instance()};
   unique_ptr<EAbstractAnalytic> analytic {factory.make(type)};
   SetupAnalyticDialog dialog(analytic.get(),this);
   dialog.setWindowTitle(tr("Execute %1").arg(factory.name(type)));
   if ( dialog.exec() )
   {
      AnalyticDialog runDialog(std::move(analytic));
      runDialog.setWindowTitle(tr("Executing %1").arg(factory.name(type)));
      runDialog.exec();
   }
}






/*!
 */
void MainWindow::settingsTriggered()
{
   //TODO: open new settings dialog yet to be made.
}






/*!
 */
MainWindow::MainWindow()
{
   createActions();
   createMenus();
   _console = new QTextEdit(this);
   setCentralWidget(_console);
   restoreSettings();
}






/*!
 */
void MainWindow::createActions()
{
   EAbstractDataFactory& dataFactory {EAbstractDataFactory::instance()};
   for (quint16 i = 0; i < dataFactory.size() ;++i)
   {
      _openActions.append(new QAction(dataFactory.name(i),this));
      connect(_openActions.back(),&QAction::triggered,[this,i]{ openTriggered(i); });
   }

   EAbstractAnalyticFactory& analyticFactory {EAbstractAnalyticFactory::instance()};
   for (quint16 i = 0; i < analyticFactory.size() ;++i)
   {
      _runActions.append(new QAction(analyticFactory.name(i),this));
      connect(_runActions.back(),&QAction::triggered,[this,i]{ runTriggered(i); });
   }

   _settingsAction = new QAction(tr("&Settings"),this);
   _settingsAction->setStatusTip(tr("Open the application settings dialog."));
   connect(_settingsAction,&QAction::triggered,this,&MainWindow::settingsTriggered);

   _exitAction = new QAction(tr("&Exit"),this);
   _exitAction->setShortcut(QKeySequence::Quit);
   _exitAction->setStatusTip(tr("Exit the application."));
   connect(_exitAction,&QAction::triggered,this,&QMainWindow::close);
}






/*!
 */
void MainWindow::createMenus()
{
   QMenu* file {menuBar()->addMenu(tr("&File"))};
   file->addAction(_settingsAction);
   file->addAction(_exitAction);
   QMenu* data {menuBar()->addMenu(tr("&Open"))};
   for (auto action: _openActions)
   {
      data->addAction(action);
   }
   QMenu* analytic {menuBar()->addMenu(tr("&Execute"))};
   for (auto action: _runActions)
   {
      analytic->addAction(action);
   }
}






/*!
 */
void MainWindow::restoreSettings()
{
   QSettings settings(Ace::Settings::instance().organization()
                      ,Ace::Settings::instance().application());
   restoreState(settings.value(_stateKey).toByteArray());
   restoreGeometry(settings.value(_geometryKey).toByteArray());
}
