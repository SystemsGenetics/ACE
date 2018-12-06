#include "ace_metadatadialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeView>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include "../core/ace_metadatamodel.h"
#include "../core/emetadata.h"
#include "../core/ace_dataobject.h"
#include "ace_metadatadialog_imageviewdialog.h"



using namespace Ace;
//






/*!
 * Constructs a new metadata dialog with the given data object and system 
 * indicator. 
 *
 * @param data Pointer to the data object whose metadata will be displayed and 
 *             possibly edited by this new dialog. 
 *
 * @param system True to indicate this dialog opens the system metadata for false 
 *               for this dialog to open the user metadata. 
 */
MetadataDialog::MetadataDialog(DataObject* data, bool system):
   _model(new MetadataModel(this)),
   _system(system),
   _data(data)
{
   // .
   _view = new QTreeView;

   // .
   if ( _system )
   {
      // .
      _model->setReadOnly(true);
      _model->setMeta(_data->systemMeta());
   }

   // .
   else
   {
      // .
      _model->setMeta(_data->userMeta());

      // .
      _view->setSelectionMode(QAbstractItemView::ExtendedSelection);
      _view->setDragEnabled(true);
      _view->setAcceptDrops(true);
      _view->setDropIndicatorShown(true);
      _view->setDefaultDropAction(Qt::CopyAction);

      // .
      _view->setContextMenuPolicy(Qt::CustomContextMenu);
      connect(_view
              ,&QTreeView::customContextMenuRequested
              ,this
              ,&MetadataDialog::contextMenuRequested);
   }

   // .
   connect(_view,&QTreeView::doubleClicked,this,&MetadataDialog::indexDoubleClicked);

   // .
   _view->setModel(_model);
   createActions();
   createMenu();

   // .
   QPushButton* okButton = new QPushButton(tr("&Ok"));
   QPushButton* applyButton = new QPushButton(tr("&Apply"));
   QPushButton* cancelButton = new QPushButton(tr("&Cancel"));
   connect(okButton,&QPushButton::clicked,this,&MetadataDialog::okClicked);
   connect(applyButton,&QPushButton::clicked,this,&MetadataDialog::applyClicked);
   connect(cancelButton,&QPushButton::clicked,this,&QDialog::reject);

   // .
   if ( _system )
   {
      // .
      okButton->setDisabled(true);
      applyButton->setDisabled(true);
   }

   // .
   QHBoxLayout* buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget(okButton);
   buttonLayout->addWidget(applyButton);
   buttonLayout->addStretch();
   buttonLayout->addWidget(cancelButton);

   // .
   QVBoxLayout* layout = new QVBoxLayout;
   layout->addWidget(_view);
   layout->addLayout(buttonLayout);
   setLayout(layout);
}






/*!
 * Called when the add action is triggered, adding a new metadata object with the 
 * given type. 
 *
 * @param type The metadata type of the new metadata object added. 
 */
void MetadataDialog::addTriggered(int type)
{
   // .
   _model->insert(_lastIndex,static_cast<EMetadata::Type>(type));
}






/*!
 * Called when the remove action is triggered. This queries the user if they are 
 * sure about removing the metadata, and removes the last selected index if the 
 * user confirms removal. 
 */
void MetadataDialog::removeTriggered()
{
   // .
   QString text(tr("Are you sure you want to permanently remove this metadata?"));

   // .
   if ( _model->isContainer(_lastIndex) )
   {
      text.append(tr(" All of the metadata's children will also be removed."));
   }

   // .
   QMessageBox confirm;
   confirm.setWindowTitle(tr("Confirmation"));
   confirm.setText(text);
   confirm.setIcon(QMessageBox::Warning);
   confirm.setStandardButtons(QMessageBox::Yes|QMessageBox::No);

   // .
   if ( confirm.exec() != QMessageBox::Yes )
   {
      return;
   }

   // .
   _model->remove(_lastIndex);
}






/*!
 * Called when the set copy action is triggered. This changes its object's tree 
 * view's default drop action to copy. 
 */
void MetadataDialog::setCopyTriggered()
{
   // .
   _view->setDefaultDropAction(Qt::CopyAction);
   _setCopyAction->setChecked(true);
   _setMoveAction->setChecked(false);
}






/*!
 * Called when the set move action is triggered. This changes its object's tree 
 * view's default drop action to move. 
 */
void MetadataDialog::setMoveTriggered()
{
   // .
   _view->setDefaultDropAction(Qt::MoveAction);
   _setCopyAction->setChecked(false);
   _setMoveAction->setChecked(true);
}






/*!
 * Called when the OK button is clicked, applying the metadata of this dialog to 
 * its data object and closing this dialog with accept. This should only be done 
 * with user metadata. 
 */
void MetadataDialog::okClicked()
{
   // .
   applyClicked();
   accept();
}






/*!
 * Called when the apply button is clicked, applying the metadata of this dialog to 
 * its data object. This should only be done with user metadata. 
 */
void MetadataDialog::applyClicked()
{
   // .
   _data->setUserMeta(_model->meta());
}






/*!
 * Called when the mouse is double clicked on the given model index. If the given 
 * index is an image and it is the correct column then this opens an image view 
 * dialog for the image. 
 *
 * @param index The model index that was double clicked by the mouse. 
 */
void MetadataDialog::indexDoubleClicked(const QModelIndex& index)
{
   // .
   if ( _model->isImage(index) && index.column() == 2 )
   {
      // .
      ImageViewDialog view(index,this);
      view.exec();
   }
}






/*!
 * Called when a custom context menu is requested by this object's tree view. This 
 * runs its object's context menu at the given point. 
 *
 * @param point The point where the custom context menu is requested. 
 */
void MetadataDialog::contextMenuRequested(const QPoint& point)
{
   // .
   _lastIndex = _view->indexAt(point);

   // .
   _addMenu->setEnabled(_model->isContainer(_lastIndex));
   _removeAction->setEnabled(_lastIndex.isValid());

   // .
   _contextMenu->exec(QCursor::pos());
}






/*!
 * Create and initialize all actions for this new metadata dialog. 
 */
void MetadataDialog::createActions()
{
   // .
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

   // .
   _removeAction = new QAction(tr("&Remove"),this);
   connect(_removeAction,&QAction::triggered,this,&MetadataDialog::removeTriggered);

   // .
   _setCopyAction = new QAction(tr("&Copy"),this);
   _setCopyAction->setCheckable(true);
   _setCopyAction->setChecked(true);
   connect(_setCopyAction,&QAction::triggered,this,&MetadataDialog::setCopyTriggered);

   // .
   _setMoveAction = new QAction(tr("&Move"),this);
   _setMoveAction->setCheckable(true);
   _setMoveAction->setChecked(false);
   connect(_setMoveAction,&QAction::triggered,this,&MetadataDialog::setMoveTriggered);
}






/*!
 * Creates and initializes the custom context menu using with this new object's 
 * tree view. 
 */
void MetadataDialog::createMenu()
{
   // .
   _contextMenu = new QMenu(this);

   // .
   _addMenu = _contextMenu->addMenu(tr("&Add New"));

   // .
   for (auto action: _addActions)
   {
      _addMenu->addAction(action);
   }

   // .
   _contextMenu->addAction(_removeAction);

   // .
   QMenu* dragMenu {_contextMenu->addMenu(tr("&Drag Action"))};
   dragMenu->addAction(_setCopyAction);
   dragMenu->addAction(_setMoveAction);
}
