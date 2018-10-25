#include "ace_mainwindow.h"
#include <QAction>
#include <QMenuBar>
#include <QTextEdit>
#include <QFileDialog>
#include <QCloseEvent>
#include <QSettings>
#include "../core/eabstractdatafactory.h"
#include "../core/eabstractanalyticfactory.h"
#include "../core/ace_analytic_abstractmanager.h"
#include "../core/ace_dataobject.h"
#include "../core/ace_settings.h"
#include "ace_datawindow.h"
#include "ace_settingsdialog.h"
#include "ace_setupanalyticdialog.h"
#include "ace_analyticdialog.h"



using namespace std;
using namespace Ace;
//



/*!
 * The key used to save the state of this window in qt settings. 
 */
const char* MainWindow::_stateKey {"ace.mainwindow.state"};
/*!
 * The key used to save the geometry of this window in qt settings. 
 */
const char* MainWindow::_geometryKey {"ace.mainwindow.geometry"};
/*!
 * Pointer to the global instance of this main window. 
 */
MainWindow* MainWindow::_instance {nullptr};






/*!
 * Returns a reference to the single global instance of this main window. 
 *
 * @return Reference to the single global instance of this main window. 
 */
MainWindow& MainWindow::instance()
{
   // .
   if ( !_instance )
   {
      _instance = new MainWindow;
   }

   // .
   return *_instance;
}






/*!
 * Adds a line of text to this main window's central text edit widget that is 
 * prepended with the name of this application and "run" because it is assumed that 
 * the given string is an example analytic line command. 
 *
 * @param command The example analytic command line that is added to this main 
 *                window's test edit widget. 
 */
void MainWindow::addCommand(const QString& command)
{
   // .
   QString line
   {
      Ace::Settings::instance().application().append(" run ").append(command).append("\n")
   };

   // .
   _console->setPlainText(_console->toPlainText().append(line));
}






/*!
 * Implements _QWidget::closeEvent_. This saves the geometry and state of this main 
 * window and accepts the close event. 
 *
 * @param event The qt close event. 
 */
void MainWindow::closeEvent(QCloseEvent* event)
{
   // .
   QSettings settings(Settings::instance().organization(),Settings::instance().application());
   settings.setValue(_stateKey,saveState());
   settings.setValue(_geometryKey,saveGeometry());

   // .
   event->accept();
}






/*!
 * Called when an open action is triggered with the given data object type. This 
 * opens a file dialog selection for the user to select a data object file of the 
 * given type. If the user selects a valid file then a new data window with the 
 * associated data object is opened. 
 *
 * @param type The data object type to search for in the file selection dialog that 
 *             is opened. 
 */
void MainWindow::openTriggered(quint16 type)
{
   // .
   QFileDialog dialog(this,tr("Select File"));
   dialog.setAcceptMode(QFileDialog::AcceptOpen);

   // .
   EAbstractDataFactory& factory = EAbstractDataFactory::instance();
   dialog.setNameFilter(tr("%1 data object (*.%2)")
                        .arg(factory.name(type))
                        .arg(factory.fileExtension(type)));

   // .
   if ( !dialog.exec() )
   {
      return;
   }

   // .
   QString file = dialog.selectedFiles().first();
   DataWindow* window
   {
      new DataWindow(unique_ptr<DataObject>(new DataObject(file)),this)
   };

   // .
   QFileInfo fileInfo(file);
   window->setWindowTitle(QString("%1 (%2)").arg(fileInfo.fileName()).arg(factory.name(type)));
   window->show();
}






/*!
 * Called when a run action is triggered with the given analytic type. This opens a 
 * setup analytic dialog and if that dialog is accepted then runs the analytic with 
 * an analytic dialog. 
 *
 * @param type The analytic type that is setup and possibly executed. 
 */
void MainWindow::runTriggered(quint16 type)
{
   // .
   EAbstractAnalyticFactory& factory {EAbstractAnalyticFactory::instance()};
   unique_ptr<Analytic::AbstractManager> manager {Analytic::AbstractManager::makeManager(type,0,1)};

   // .
   SetupAnalyticDialog dialog(manager.get());
   if ( !dialog.exec() ) return;

   // .
   AnalyticDialog runDialog(std::move(manager));
   runDialog.setWindowTitle(tr("Executing %1").arg(factory.name(type)));
   runDialog.exec();
}






/*!
 * Called when the settings action is triggered. 
 */
void MainWindow::settingsTriggered()
{
   // .
   SettingsDialog dialog;
   dialog.setWindowTitle(tr("Settings"));
   dialog.exec();
}






/*!
 * Constructs a new main window instance. This is private because only the instance 
 * static method should ever create a main window. 
 */
MainWindow::MainWindow()
{
   // .
   createActions();
   createMenus();

   // .
   _console = new QTextEdit(this);
   setCentralWidget(_console);

   // .
   restoreSettings();
}






/*!
 * Creates and initializes all actions for this new main window. 
 */
void MainWindow::createActions()
{
   // .
   EAbstractDataFactory& dataFactory {EAbstractDataFactory::instance()};
   for (quint16 i = 0; i < dataFactory.size() ;++i)
   {
      _openActions.append(new QAction(dataFactory.name(i),this));
      connect(_openActions.back(),&QAction::triggered,[this,i]{ openTriggered(i); });
   }

   // .
   EAbstractAnalyticFactory& analyticFactory {EAbstractAnalyticFactory::instance()};
   for (quint16 i = 0; i < analyticFactory.size() ;++i)
   {
      _runActions.append(new QAction(analyticFactory.name(i),this));
      connect(_runActions.back(),&QAction::triggered,[this,i]{ runTriggered(i); });
   }

   // .
   _settingsAction = new QAction(tr("&Settings"),this);
   _settingsAction->setStatusTip(tr("Open the application settings dialog."));
   connect(_settingsAction,&QAction::triggered,this,&MainWindow::settingsTriggered);

   // .
   _exitAction = new QAction(tr("&Exit"),this);
   _exitAction->setShortcut(QKeySequence::Quit);
   _exitAction->setStatusTip(tr("Exit the application."));
   connect(_exitAction,&QAction::triggered,this,&QMainWindow::close);
}






/*!
 * Creates and initializes all menus for this new main window. 
 */
void MainWindow::createMenus()
{
   // .
   QMenu* file {menuBar()->addMenu(tr("&File"))};

   // .
   file->addAction(_settingsAction);
   file->addAction(_exitAction);

   // .
   QMenu* data {menuBar()->addMenu(tr("&Open"))};
   for (auto action: _openActions)
   {
      data->addAction(action);
   }

   // .
   QMenu* analytic {menuBar()->addMenu(tr("&Execute"))};
   for (auto action: _runActions)
   {
      analytic->addAction(action);
   }
}






/*!
 * Restores the geometry and state of this new main window. 
 */
void MainWindow::restoreSettings()
{
   // .
   QSettings settings(Settings::instance().organization(),Settings::instance().application());
   restoreState(settings.value(_stateKey).toByteArray());
   restoreGeometry(settings.value(_geometryKey).toByteArray());
}
