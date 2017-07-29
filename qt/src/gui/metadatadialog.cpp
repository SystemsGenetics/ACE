#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeView>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>

#include "metadatadialog.h"
#include "metadatamodel.h"
#include "metadata.h"
#include "dataobject.h"
#include "imageviewer.h"



using namespace Ace;



MetadataDialog::MetadataDialog(DataObject* data, QWidget* parent):
   QDialog(parent),
   _model(data->getModel()),
   _data(data)
{
   // create actions and menus
   createActions();
   createMenus();

   // create and configure tree view that displays metadata model
   _view = new QTreeView;
   _view->setModel(_model);
   _view->setSelectionMode(QAbstractItemView::ExtendedSelection);
   _view->setDragEnabled(true);
   _view->setAcceptDrops(true);
   _view->setDropIndicatorShown(true);
   _view->setDefaultDropAction(Qt::CopyAction);
   _view->setContextMenuPolicy(Qt::CustomContextMenu);

   // create lower buttons
   QPushButton* okButton = new QPushButton(tr("&Ok"));
   QPushButton* applyButton = new QPushButton(tr("&Apply"));
   QPushButton* cancelButton = new QPushButton(tr("&Cancel"));

   // make lower buttons layout
   QHBoxLayout* buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget(okButton);
   buttonLayout->addWidget(applyButton);
   buttonLayout->addStretch();
   buttonLayout->addWidget(cancelButton);

   // make main layout
   QVBoxLayout* layout = new QVBoxLayout;
   layout->addWidget(_view);
   layout->addLayout(buttonLayout);
   setLayout(layout);

   // connect all signals
   connect(_view,SIGNAL(customContextMenuRequested(QPoint)),this
           ,SLOT(metadataContextMenuRequested(QPoint)));
   connect(_view,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(indexDoubleClicked(QModelIndex)));
   connect(okButton,SIGNAL(clicked(bool)),this,SLOT(accept()));
   connect(applyButton,SIGNAL(clicked(bool)),this,SLOT(applyClicked()));
   connect(cancelButton,SIGNAL(clicked(bool)),this,SLOT(reject()));
}






void MetadataDialog::metadataContextMenuRequested(const QPoint &point)
{
   // get index where cursor right-clicked and save it
   QModelIndex index = _view->indexAt(point);
   _lastIndex = index;

   // enable/disable add menu and remove action depending on index type
   _addMenu->setEnabled(_model->isInsertable(index));
   _removeAction->setEnabled(index.isValid());

   // execute custom menu at cursor location
   _mainMenu->exec(QCursor::pos());
}






void MetadataDialog::addTriggered()
{
   // get new metadata type from action and add it to model
   QAction* from = qobject_cast<QAction*>(sender());
   _model->insertRow(-1,new EMetadata(static_cast<EMetadata::Type>(from->data().toInt()))
                     ,_lastIndex);
}






void MetadataDialog::removeTriggered()
{
   // make confirmation dialog for user making sure user wants to remove metadata
   QString text(tr("Are you sure you want to permanently remove this metadata?"));
   if ( _model->rowCount(_lastIndex) > 0 )
   {
      // if metadata has children highlight that to user
      text.append(tr(" All of the metadata's children will also be removed."));
   }
   QMessageBox confirm;
   confirm.setWindowTitle(tr("Confirmation"));
   confirm.setText(text);
   confirm.setIcon(QMessageBox::Warning);
   confirm.setStandardButtons(QMessageBox::Yes|QMessageBox::No);

   // if user confirms deletion remove metadata from model
   if ( confirm.exec() == QMessageBox::Yes )
   {
      _model->removeRow(_lastIndex.row(),_model->parent(_lastIndex));
   }
}






void MetadataDialog::setCopyTriggered()
{
   // set copy action as default and flip checkboxes
   _view->setDefaultDropAction(Qt::CopyAction);
   _setCopyAction->setChecked(true);
   _setMoveAction->setChecked(false);
}






void MetadataDialog::setMoveTriggered()
{
   // set move action as default and flip checkboxes
   _view->setDefaultDropAction(Qt::MoveAction);
   _setCopyAction->setChecked(false);
   _setMoveAction->setChecked(true);
}






void MetadataDialog::applyClicked()
{
   _data->writeMeta();
}






void MetadataDialog::indexDoubleClicked(const QModelIndex &index)
{
   // if index is an image and third column was double clicked open image viewer
   if ( _model->isImage(index) && index.column() == 2 )
   {
      ImageViewer view(_model,index,this);
      view.exec();
   }
}






void MetadataDialog::accept()
{
   // write metadata to file and close
   _data->writeMeta();
   QDialog::accept();
}






void MetadataDialog::reject()
{
   // reload metadata from file discarding any chages and close
   _data->reloadMeta();
   QDialog::reject();
}






void MetadataDialog::createActions()
{
   // create all actions for adding new metadata of any type
   _addActions.append(new QAction(tr("Null"),this));
   _addActions.back()->setData(EMetadata::Null);
   connect(_addActions.back(),SIGNAL(triggered(bool)),this,SLOT(addTriggered()));
   _addActions.append(new QAction(tr("Boolean"),this));
   _addActions.back()->setData(EMetadata::Bool);
   connect(_addActions.back(),SIGNAL(triggered(bool)),this,SLOT(addTriggered()));
   _addActions.append(new QAction(tr("Real"),this));
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

   // create remove metadata action
   _removeAction = new QAction(tr("&Remove"),this);
   connect(_removeAction,SIGNAL(triggered(bool)),this,SLOT(removeTriggered()));

   // create set drag as copy action
   _setCopyAction = new QAction(tr("&Copy"),this);
   _setCopyAction->setCheckable(true);
   _setCopyAction->setChecked(true);
   connect(_setCopyAction,SIGNAL(triggered(bool)),this,SLOT(setCopyTriggered()));

   // create set drag as move action
   _setMoveAction = new QAction(tr("&Move"),this);
   _setMoveAction->setCheckable(true);
   _setMoveAction->setChecked(false);
   connect(_setMoveAction,SIGNAL(triggered(bool)),this,SLOT(setMoveTriggered()));
}






void MetadataDialog::createMenus()
{
   // create context menu used in tree view
   _mainMenu = new QMenu(this);

   // create add new submenu and add all possible metadata types
   _addMenu = _mainMenu->addMenu(tr("&Add New"));
   _addMenu->addAction(_addActions.at(EMetadata::Null));
   _addMenu->addAction(_addActions.at(EMetadata::Bool));
   _addMenu->addAction(_addActions.at(EMetadata::Double));
   _addMenu->addAction(_addActions.at(EMetadata::String));
   _addMenu->addAction(_addActions.at(EMetadata::Bytes));
   _addMenu->addAction(_addActions.at(EMetadata::Array));
   _addMenu->addAction(_addActions.at(EMetadata::Object));

   // add remove action to main
   _mainMenu->addAction(_removeAction);

   // add drag action submenu to main
   _dragMenu = _mainMenu->addMenu(tr("&Drag Action"));
   _dragMenu->addAction(_setCopyAction);
   _dragMenu->addAction(_setMoveAction);
}
