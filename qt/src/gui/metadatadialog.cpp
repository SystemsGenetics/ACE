#include <QVBoxLayout>
#include <QTreeView>
#include <QMenu>

#include "metadatadialog.h"
#include "metadatamodel.h"
#include "metadata.h"



using namespace Ace;



MetadataDialog::MetadataDialog(MetadataModel *model, QWidget *parent):
   QDialog(parent),
   _model(model)
{
   QVBoxLayout* layout = new QVBoxLayout;
   _view = new QTreeView;
   _view->setModel(_model);
   _view->setSelectionMode(QAbstractItemView::ExtendedSelection);
   _view->setDragEnabled(true);
   _view->setAcceptDrops(true);
   _view->setDropIndicatorShown(true);
   _view->setDefaultDropAction(Qt::MoveAction);
   _view->setContextMenuPolicy(Qt::CustomContextMenu);
   connect(_view,SIGNAL(customContextMenuRequested(QPoint)),this
           ,SLOT(metadataContextMenuRequested(QPoint)));
   layout->addWidget(_view);
   setLayout(layout);
   createActions();
   createMenus();
}






void MetadataDialog::metadataContextMenuRequested(const QPoint &point)
{
   QModelIndex index = _view->indexAt(point);
   _lastIndex = index;
   _addMenu->setEnabled(_model->isInsertable(index));
   _mainMenu->exec(QCursor::pos());
}






void MetadataDialog::addTriggered()
{
   QAction* from = qobject_cast<QAction*>(sender());
   _model->insertRow(-1,new EMetadata(static_cast<EMetadata::Type>(from->data().toInt()))
                     ,_lastIndex);
}






void MetadataDialog::createActions()
{
   _addActions.append(new QAction(tr("Null"),this));
   _addActions.back()->setData(EMetadata::Null);
   connect(_addActions.back(),SIGNAL(triggered(bool)),this,SLOT(addTriggered()));
   _addActions.append(new QAction(tr("Boolean"),this));
   _addActions.back()->setData(EMetadata::Bool);
   connect(_addActions.back(),SIGNAL(triggered(bool)),this,SLOT(addTriggered()));
   _addActions.append(new QAction(tr("Double"),this));
   _addActions.back()->setData(EMetadata::Double);
   connect(_addActions.back(),SIGNAL(triggered(bool)),this,SLOT(addTriggered()));
   _addActions.append(new QAction(tr("String"),this));
   _addActions.back()->setData(EMetadata::String);
   connect(_addActions.back(),SIGNAL(triggered(bool)),this,SLOT(addTriggered()));
   _addActions.append(new QAction(tr("Image"),this));
   _addActions.back()->setData(EMetadata::Bytes);
   connect(_addActions.back(),SIGNAL(triggered(bool)),this,SLOT(addTriggered()));
   _addActions.append(new QAction(tr("Array"),this));
   _addActions.back()->setData(EMetadata::Array);
   connect(_addActions.back(),SIGNAL(triggered(bool)),this,SLOT(addTriggered()));
   _addActions.append(new QAction(tr("Object"),this));
   _addActions.back()->setData(EMetadata::Object);
   connect(_addActions.back(),SIGNAL(triggered(bool)),this,SLOT(addTriggered()));
}






void MetadataDialog::createMenus()
{
   _mainMenu = new QMenu(this);
   _addMenu = _mainMenu->addMenu(tr("&Add New"));
   _addMenu->addAction(_addActions.at(EMetadata::Null));
   _addMenu->addAction(_addActions.at(EMetadata::Bool));
   _addMenu->addAction(_addActions.at(EMetadata::Double));
   _addMenu->addAction(_addActions.at(EMetadata::String));
   _addMenu->addAction(_addActions.at(EMetadata::Bytes));
   _addMenu->addAction(_addActions.at(EMetadata::Array));
   _addMenu->addAction(_addActions.at(EMetadata::Object));
}
