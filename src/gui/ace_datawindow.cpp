#include "ace_datawindow.h"
#include <QTableView>
#include <QAction>
#include <QMenuBar>
#include <QSettings>
#include <../core/ace_dataobject.h>
#include <../core/ace_settings.h>
#include <../core/eabstractdata.h>
#include "ace_metadatadialog.h"



using namespace Ace;
//



/*!
 */
const char* DataWindow::_stateKey {"ace.datawindow.state"};
/*!
 */
const char* DataWindow::_geometryKey {"ace.datawindow.geometry"};






/*!
 *
 * @param data  
 *
 * @param parent  
 */
DataWindow::DataWindow(std::unique_ptr<DataObject>&& data, QWidget* parent):
   QMainWindow(parent),
   _data(data.get())
{
   data.release()->setParent(this);
   createActions();
   createMenu();

   QTableView* _view = new QTableView;
   _view->setModel(_data->data()->model());
   connect(_data,&DataObject::overwritten,this,&QMainWindow::close);
   setCentralWidget(_view);

   restoreSettings();
}






/*!
 */
DataWindow::~DataWindow()
{
   QSettings settings(Settings::instance().organization(),Settings::instance().application());
   settings.setValue(_stateKey,saveState());
   settings.setValue(_geometryKey,saveGeometry());
}






/*!
 *
 * @param title  
 */
void DataWindow::setWindowTitle(const QString& title)
{
   _title = title;
   QWidget::setWindowTitle(title);
}






/*!
 */
void DataWindow::systemMetaTriggered()
{
   MetadataDialog dialog(_data);
   dialog.exec();
}






/*!
 */
void DataWindow::userMetaTriggered()
{
   MetadataDialog dialog(_data,false);
   dialog.exec();
}






/*!
 */
void DataWindow::createActions()
{
   _systemMetaAction = new QAction(tr("&System Metadata"),this);
   _systemMetaAction->setStatusTip(tr("Open the system metadata browser for this data object."));
   connect(_systemMetaAction,&QAction::triggered,this,&DataWindow::systemMetaTriggered);

   _userMetaAction = new QAction(tr("&User Metadata"),this);
   _userMetaAction->setStatusTip(tr("Open the user metadata browser for this data object."));
   connect(_userMetaAction,&QAction::triggered,this,&DataWindow::userMetaTriggered);

   _closeAction = new QAction(tr("&Close"),this);
   _closeAction->setStatusTip(tr("Close this data object's window."));
   connect(_closeAction,&QAction::triggered,this,&QMainWindow::close);
}






/*!
 */
void DataWindow::createMenu()
{
   QMenu* file = menuBar()->addMenu(tr("&File"));
   file->addAction(_systemMetaAction);
   file->addAction(_userMetaAction);
   file->addAction(_closeAction);
}






/*!
 */
void DataWindow::restoreSettings()
{
   QSettings settings(Settings::instance().organization(),Settings::instance().application());
   restoreState(settings.value(_stateKey).toByteArray());
   restoreGeometry(settings.value(_geometryKey).toByteArray());
}
