#include <QTreeView>
#include <QAction>
#include <QMenuBar>

#include "datawindow.h"
#include "abstractdata.h"
#include "datareference.h"
#include "dataobject.h"
#include "metadatadialog.h"



using namespace Ace;






DataWindow::DataWindow(Ace::DataReference* data, QWidget *parent):
   QMainWindow(parent),
   _data(data)
{
   // open data object and create actions and menus
   (*data)->open();
   createActions();
   createMenus();

   // create central tree view widget and set model to data's
   QTreeView* _view = new QTreeView;
   _view->setModel((*_data)->data().getModel());
   setCentralWidget(_view);
}






DataWindow::~DataWindow()
{
   delete _data;
}






void DataWindow::setWindowTitle(const QString& title)
{
   // save title then set it
   _title = title;
   QWidget::setWindowTitle(title);
}






void DataWindow::metadataTriggered()
{
   // create metadata dialog editor and modally run it
   MetadataDialog dialog(&(**_data),this);
   dialog.setWindowTitle(tr("%1 Metadata").arg(_title));
   dialog.exec();
}






void DataWindow::createActions()
{
   // create metadata action
   _metadataAction = new QAction(tr("&Metadata"),this);
   _metadataAction->setStatusTip(tr("Open metadata browser for this data object."));
   connect(_metadataAction,SIGNAL(triggered(bool)),this,SLOT(metadataTriggered()));

   // create metadata action
   _closeAction = new QAction(tr("&Close"),this);
   _closeAction->setStatusTip(tr("Close this data object's window."));
   connect(_closeAction,SIGNAL(triggered(bool)),this,SLOT(close()));
}






void DataWindow::createMenus()
{
   // add file menu
   _fileMenu = menuBar()->addMenu(tr("&File"));
   _fileMenu->addAction(_metadataAction);
   _fileMenu->addAction(_closeAction);
}
