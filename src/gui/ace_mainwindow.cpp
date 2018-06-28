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
   // If the global instance has no been created yet then do so.
   if ( !_instance )
   {
      _instance = new MainWindow;
   }

   // Return a reference to the global instance.
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
   // Create and initialize the line that will be added to this window.
   QString line
   {
      Ace::Settings::instance().application().append(" run ").append(command).append("\n")
   };

   // Append the new line to this window's central text edit widget.
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
   // Save the geometry and state of this main window using qt settings.
   QSettings settings(Settings::instance().organization(),Settings::instance().application());
   settings.setValue(_stateKey,saveState());
   settings.setValue(_geometryKey,saveGeometry());

   // Accept this close event.
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
   // Create a new file dialog and set it for accepting open.
   QFileDialog dialog(this,tr("Select File"));
   dialog.setAcceptMode(QFileDialog::AcceptOpen);

   // Set the name filter for the file dialog based off the given data object type.
   EAbstractDataFactory& factory = EAbstractDataFactory::instance();
   dialog.setNameFilter(tr("%1 data object (*.%2)")
                        .arg(factory.name(type))
                        .arg(factory.fileExtension(type)));

   // Execute the file dialog selection for the user. If the dialog is rejected then return.
   if ( !dialog.exec() )
   {
      return;
   }

   // Get the first file selected from the file dialog and create a new data window using
   // the selected file.
   QString file = dialog.selectedFiles().first();
   DataWindow* window
   {
      new DataWindow(unique_ptr<DataObject>(new DataObject(file)),this)
   };

   // Set the title of this new data window and show it.
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
   // Create a new analytic manager with the given analytic type and index/size parameters so
   // this is not chunk mode.
   EAbstractAnalyticFactory& factory {EAbstractAnalyticFactory::instance()};
   unique_ptr<Analytic::AbstractManager> manager {Analytic::AbstractManager::makeManager(type,0,1)};

   // Create and execute a setup analytic dialog with the analytic manager. If the dialog is
   // rejected then return.
   SetupAnalyticDialog dialog(manager.get());
   if ( !dialog.exec() ) return;

   // Create a new analytic dialog, set its title, and execute the analytic manager with it.
   AnalyticDialog runDialog(std::move(manager));
   runDialog.setWindowTitle(tr("Executing %1").arg(factory.name(type)));
   runDialog.exec();
}






/*!
 * Called when the settings action is triggered. 
 */
void MainWindow::settingsTriggered()
{
   // Create a settings dialog, setting its title and executing it.
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
   // Create this new window's actions and menus.
   createActions();
   createMenus();

   // Create this new window's text edit widget and set it as the central widget.
   _console = new QTextEdit(this);
   setCentralWidget(_console);

   // Restore this window's geometry and state.
   restoreSettings();
}






/*!
 * Creates and initializes all actions for this new main window. 
 */
void MainWindow::createActions()
{
   // Create and initialize the list of open actions corresponding to all available data
   // object types.
   EAbstractDataFactory& dataFactory {EAbstractDataFactory::instance()};
   for (quint16 i = 0; i < dataFactory.size() ;++i)
   {
      _openActions.append(new QAction(dataFactory.name(i),this));
      connect(_openActions.back(),&QAction::triggered,[this,i]{ openTriggered(i); });
   }

   // Create and initialize the list of run actions corresponding to all available analytic
   // types.
   EAbstractAnalyticFactory& analyticFactory {EAbstractAnalyticFactory::instance()};
   for (quint16 i = 0; i < analyticFactory.size() ;++i)
   {
      _runActions.append(new QAction(analyticFactory.name(i),this));
      connect(_runActions.back(),&QAction::triggered,[this,i]{ runTriggered(i); });
   }

   // Create and initialize the settings action.
   _settingsAction = new QAction(tr("&Settings"),this);
   _settingsAction->setStatusTip(tr("Open the application settings dialog."));
   connect(_settingsAction,&QAction::triggered,this,&MainWindow::settingsTriggered);

   // Create and initialize the exit action.
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
   // Create the file menu.
   QMenu* file {menuBar()->addMenu(tr("&File"))};

   // Add the settings and exit actions to the file menu.
   file->addAction(_settingsAction);
   file->addAction(_exitAction);

   // Create the open menu and add all open actions to it.
   QMenu* data {menuBar()->addMenu(tr("&Open"))};
   for (auto action: _openActions)
   {
      data->addAction(action);
   }

   // Create the execute menu and add all run actions to it.
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
   // Using qt settings restore the geometry and state of this window.
   QSettings settings(Settings::instance().organization(),Settings::instance().application());
   restoreState(settings.value(_stateKey).toByteArray());
   restoreGeometry(settings.value(_geometryKey).toByteArray());
}
