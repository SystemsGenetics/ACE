#include "ace_metadatadialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeView>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <core/ace_metadatamodel.h>
#include <core/emetadata.h>
#include <core/ace_dataobject.h>
#include "ace_metadatadialog_imageviewdialog.h"



using namespace Ace;
//






/*!
 *
 * @param data  
 *
 * @param system  
 */
MetadataDialog::MetadataDialog(DataObject* data, bool system):
   _model(new MetadataModel(this)),
   _system(system),
   _data(data)
{
   _view = new QTreeView;
   if ( _system )
   {
      _model->setMeta(_data->systemMeta());
   }
   else
   {
      _model->setMeta(_data->userMeta());
      _view->setSelectionMode(QAbstractItemView::ExtendedSelection);
      _view->setDragEnabled(true);
      _view->setAcceptDrops(true);
      _view->setDropIndicatorShown(true);
      _view->setDefaultDropAction(Qt::CopyAction);
      _view->setContextMenuPolicy(Qt::CustomContextMenu);
      connect(_view
              ,&QTreeView::customContextMenuRequested
              ,this
              ,&MetadataDialog::contextMenuRequested);
   }
   connect(_view,&QTreeView::doubleClicked,this,&MetadataDialog::indexDoubleClicked);
   _view->setModel(_model);
   createActions();
   createMenu();

   QPushButton* okButton = new QPushButton(tr("&Ok"));
   QPushButton* applyButton = new QPushButton(tr("&Apply"));
   QPushButton* cancelButton = new QPushButton(tr("&Cancel"));
   connect(okButton,&QPushButton::clicked,this,&MetadataDialog::okClicked);
   connect(applyButton,&QPushButton::clicked,this,&MetadataDialog::applyClicked);
   connect(cancelButton,&QPushButton::clicked,this,&QDialog::reject);

   QHBoxLayout* buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget(okButton);
   buttonLayout->addWidget(applyButton);
   buttonLayout->addStretch();
   buttonLayout->addWidget(cancelButton);

   QVBoxLayout* layout = new QVBoxLayout;
   layout->addWidget(_view);
   layout->addLayout(buttonLayout);
   setLayout(layout);
}






/*!
 *
 * @param type  
 */
void MetadataDialog::addTriggered(int type)
{
   _model->insert(_lastIndex,static_cast<EMetadata::Type>(type));
}






/*!
 */
void MetadataDialog::removeTriggered()
{
   QString text(tr("Are you sure you want to permanently remove this metadata?"));
   if ( _model->isContainer(_lastIndex) )
   {
      text.append(tr(" All of the metadata's children will also be removed."));
   }

   QMessageBox confirm;
   confirm.setWindowTitle(tr("Confirmation"));
   confirm.setText(text);
   confirm.setIcon(QMessageBox::Warning);
   confirm.setStandardButtons(QMessageBox::Yes|QMessageBox::No);

   if ( confirm.exec() == QMessageBox::Yes )
   {
      _model->remove(_lastIndex);
   }
}






/*!
 */
void MetadataDialog::setCopyTriggered()
{
   _view->setDefaultDropAction(Qt::CopyAction);
   _setCopyAction->setChecked(true);
   _setMoveAction->setChecked(false);
}






/*!
 */
void MetadataDialog::setMoveTriggered()
{
   _view->setDefaultDropAction(Qt::MoveAction);
   _setCopyAction->setChecked(false);
   _setMoveAction->setChecked(true);
}






/*!
 */
void MetadataDialog::okClicked()
{
   applyClicked();
   accept();
}






/*!
 */
void MetadataDialog::applyClicked()
{
   _data->setUserMeta(_model->meta());
}






/*!
 *
 * @param index  
 */
void MetadataDialog::indexDoubleClicked(const QModelIndex& index)
{
   if ( _model->isImage(index) && index.column() == 2 )
   {
      ImageViewDialog view(index,this);
      view.exec();
   }
}






/*!
 *
 * @param point  
 */
void MetadataDialog::contextMenuRequested(const QPoint& point)
{
   _lastIndex = _view->indexAt(point);
   _addMenu->setEnabled(_model->isContainer(_lastIndex));
   _removeAction->setEnabled(_lastIndex.isValid());
   _contextMenu->exec(QCursor::pos());
}






/*!
 */
void MetadataDialog::createActions()
{
   // create all actions for adding new metadata of any type
   _addActions.append(new QAction(tr("Null"),this));
   connect(_addActions.back(),&QAction::triggered,this,[this]{ addTriggered(EMetadata::Null); });
   _addActions.append(new QAction(tr("Boolean"),this));
   connect(_addActions.back(),&QAction::triggered,this,[this]{ addTriggered(EMetadata::Bool); });
   _addActions.append(new QAction(tr("Real Number"),this));
   connect(_addActions.back(),&QAction::triggered,this,[this]{ addTriggered(EMetadata::Double); });
   _addActions.append(new QAction(tr("String"),this));
   connect(_addActions.back(),&QAction::triggered,this,[this]{ addTriggered(EMetadata::String); });
   _addActions.append(new QAction(tr("Image"),this));
   connect(_addActions.back(),&QAction::triggered,this,[this]{ addTriggered(EMetadata::Bytes); });
   _addActions.append(new QAction(tr("Array"),this));
   connect(_addActions.back(),&QAction::triggered,this,[this]{ addTriggered(EMetadata::Array); });
   _addActions.append(new QAction(tr("Object"),this));
   connect(_addActions.back(),&QAction::triggered,this,[this]{ addTriggered(EMetadata::Object); });

   // create remove metadata action
   _removeAction = new QAction(tr("&Remove"),this);
   connect(_removeAction,&QAction::triggered,this,&MetadataDialog::removeTriggered);

   // create set drag as copy action
   _setCopyAction = new QAction(tr("&Copy"),this);
   _setCopyAction->setCheckable(true);
   _setCopyAction->setChecked(true);
   connect(_setCopyAction,&QAction::triggered,this,&MetadataDialog::setCopyTriggered);

   // create set drag as move action
   _setMoveAction = new QAction(tr("&Move"),this);
   _setMoveAction->setCheckable(true);
   _setMoveAction->setChecked(false);
   connect(_setMoveAction,&QAction::triggered,this,&MetadataDialog::setMoveTriggered);
}






/*!
 */
void MetadataDialog::createMenu()
{
   _contextMenu = new QMenu(this);

   _addMenu = _contextMenu->addMenu(tr("&Add New"));
   for (auto action: _addActions)
   {
      _addMenu->addAction(action);
   }

   _contextMenu->addAction(_removeAction);
   QMenu* dragMenu {_contextMenu->addMenu(tr("&Drag Action"))};
   dragMenu->addAction(_setCopyAction);
   dragMenu->addAction(_setMoveAction);
}
