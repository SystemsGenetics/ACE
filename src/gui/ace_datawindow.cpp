#include "ace_datawindow.h"
#include <QTableView>
#include <QAction>
#include <QMenuBar>
#include <QSettings>
#include <core/ace_dataobject.h>
#include <core/ace_settings.h>
#include <core/eabstractdata.h>
#include "metadatadialog.h"



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
void DataWindow::metadataTriggered()
{
   //TODO: metadata dialog
}






/*!
 */
void DataWindow::createActions()
{
   _metadataAction = new QAction(tr("&Metadata"),this);
   _metadataAction->setStatusTip(tr("Open metadata browser for this data object."));
   connect(_metadataAction,&QAction::triggered,this,&DataWindow::metadataTriggered);

   _closeAction = new QAction(tr("&Close"),this);
   _closeAction->setStatusTip(tr("Close this data object's window."));
   connect(_closeAction,&QAction::triggered,this,&QMainWindow::close);
}






/*!
 */
void DataWindow::createMenu()
{
   QMenu* file = menuBar()->addMenu(tr("&File"));
   file->addAction(_metadataAction);
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
