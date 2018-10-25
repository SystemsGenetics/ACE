#include "ace_datawindow.h"
#include <QTableView>
#include <QAction>
#include <QMenuBar>
#include <QSettings>
#include <QCloseEvent>
#include "../core/ace_dataobject.h"
#include "../core/ace_settings.h"
#include "../core/eabstractdata.h"
#include "ace_metadatadialog.h"



using namespace Ace;
//



/*!
 * The key used to save the state of this data window in qt settings. 
 */
const char* DataWindow::_stateKey {"ace.datawindow.state"};
/*!
 * The key used to save the geometry of this data window in qt settings. 
 */
const char* DataWindow::_geometryKey {"ace.datawindow.geometry"};






/*!
 * Constructs a new data window with the given data object and optional parent. 
 *
 * @param data The data object that this new data window takes ownership of and 
 *             displays to the user. 
 *
 * @param parent The optional parent for this new window. 
 */
DataWindow::DataWindow(std::unique_ptr<DataObject>&& data, QWidget* parent):
   QMainWindow(parent),
   _data(data.get())
{
   // .
   data.release()->setParent(this);

   // .
   connect(_data,&DataObject::overwritten,this,&QMainWindow::close);

   // .
   createActions();
   createMenu();

   // .
   QTableView* _view = new QTableView;
   _view->setModel(_data->data()->model());
   setCentralWidget(_view);

   // .
   restoreSettings();
}






/*!
 * Implements _QWidget::closeEvent_. This saves the geometry and state of this 
 * window and accepts the close event. 
 *
 * @param event The qt close event. 
 */
void DataWindow::closeEvent(QCloseEvent* event)
{
   // .
   QSettings settings(Settings::instance().organization(),Settings::instance().application());
   settings.setValue(_stateKey,saveState());
   settings.setValue(_geometryKey,saveGeometry());

   // .
   event->accept();
}






/*!
 * Called when the system metadata action is triggered, opening a system metadata 
 * dialog. 
 */
void DataWindow::systemMetaTriggered()
{
   // .
   MetadataDialog dialog(_data);
   dialog.setWindowTitle(tr("System Metadata"));
   dialog.exec();
}






/*!
 * Called when the user metadata action is triggered, opening a user metadata 
 * dialog. 
 */
void DataWindow::userMetaTriggered()
{
   // .
   MetadataDialog dialog(_data,false);
   dialog.setWindowTitle(tr("User Metadata"));
   dialog.exec();
}






/*!
 * Creates and initializes all actions for this new data window. 
 */
void DataWindow::createActions()
{
   // .
   _systemMetaAction = new QAction(tr("&System Metadata"),this);
   _systemMetaAction->setStatusTip(tr("Open the system metadata browser for this data object."));
   connect(_systemMetaAction,&QAction::triggered,this,&DataWindow::systemMetaTriggered);

   // .
   _userMetaAction = new QAction(tr("&User Metadata"),this);
   _userMetaAction->setStatusTip(tr("Open the user metadata browser for this data object."));
   connect(_userMetaAction,&QAction::triggered,this,&DataWindow::userMetaTriggered);

   // .
   _closeAction = new QAction(tr("&Close"),this);
   _closeAction->setStatusTip(tr("Close this data object's window."));
   connect(_closeAction,&QAction::triggered,this,&QMainWindow::close);
}






/*!
 * Creates and initializes the file menu for this new data window. 
 */
void DataWindow::createMenu()
{
   // .
   QMenu* file = menuBar()->addMenu(tr("&File"));
   file->addAction(_systemMetaAction);
   file->addAction(_userMetaAction);
   file->addAction(_closeAction);
}






/*!
 * Restores the geometry and state of this data window. 
 */
void DataWindow::restoreSettings()
{
   // .
   QSettings settings(Settings::instance().organization(),Settings::instance().application());
   restoreState(settings.value(_stateKey).toByteArray());
   restoreGeometry(settings.value(_geometryKey).toByteArray());
}
