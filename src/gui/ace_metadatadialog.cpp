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
   // Create the tree view.
   _view = new QTreeView;

   // Check if this dialog will use the system metadata.
   if ( _system )
   {
      // Set the model as read only and read in the system metadata.
      _model->setReadOnly(true);
      _model->setMeta(_data->systemMeta());
   }

   // Else this dialog will use the user metadata.
   else
   {
      // Read in the system metadata.
      _model->setMeta(_data->userMeta());

      // Initialize the tree view for drag and drop actions.
      _view->setSelectionMode(QAbstractItemView::ExtendedSelection);
      _view->setDragEnabled(true);
      _view->setAcceptDrops(true);
      _view->setDropIndicatorShown(true);
      _view->setDefaultDropAction(Qt::CopyAction);

      // Set the tree view context menu policy as custom and connect its signal.
      _view->setContextMenuPolicy(Qt::CustomContextMenu);
      connect(_view
              ,&QTreeView::customContextMenuRequested
              ,this
              ,&MetadataDialog::contextMenuRequested);
   }

   // Connect the double clicked signal.
   connect(_view,&QTreeView::doubleClicked,this,&MetadataDialog::indexDoubleClicked);

   // Set the tree view's model and create this new dialog's actions and custom context menu.
   _view->setModel(_model);
   createActions();
   createMenu();

   // Create this new dialog's buttons and connect their clicked signals.
   QPushButton* okButton = new QPushButton(tr("&Ok"));
   QPushButton* applyButton = new QPushButton(tr("&Apply"));
   QPushButton* cancelButton = new QPushButton(tr("&Cancel"));
   connect(okButton,&QPushButton::clicked,this,&MetadataDialog::okClicked);
   connect(applyButton,&QPushButton::clicked,this,&MetadataDialog::applyClicked);
   connect(cancelButton,&QPushButton::clicked,this,&QDialog::reject);

   // Check if this dialog uses system metadata.
   if ( _system )
   {
      // Disable the ok and apply buttons.
      okButton->setDisabled(true);
      applyButton->setDisabled(true);
   }

   // Create the button layout and add all buttons to it.
   QHBoxLayout* buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget(okButton);
   buttonLayout->addWidget(applyButton);
   buttonLayout->addStretch();
   buttonLayout->addWidget(cancelButton);

   // Create this dialog's main layout, add the tree view and the button layout, and then set
   // this new dialog's main layout.
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
   // Insert a new metadata value of the given type into the last model index.
   _model->insert(_lastIndex,static_cast<EMetadata::Type>(type));
}






/*!
 * Called when the remove action is triggered. This queries the user if they are 
 * sure about removing the metadata, and removes the last selected index if the 
 * user confirms removal. 
 */
void MetadataDialog::removeTriggered()
{
   // Construct the question presented to the user about removing the metadata.
   QString text(tr("Are you sure you want to permanently remove this metadata?"));

   // If the metadata that might be removed is a container add that information to the question
   // presented to the user.
   if ( _model->isContainer(_lastIndex) )
   {
      text.append(tr(" All of the metadata's children will also be removed."));
   }

   // Create a confirm dialog with the user question and yes/no buttons.
   QMessageBox confirm;
   confirm.setWindowTitle(tr("Confirmation"));
   confirm.setText(text);
   confirm.setIcon(QMessageBox::Warning);
   confirm.setStandardButtons(QMessageBox::Yes|QMessageBox::No);

   // Call modal execution on the confirm dialog. If the user does not click yes then return.
   if ( confirm.exec() != QMessageBox::Yes )
   {
      return;
   }

   // Remove the metadata object at the last index.
   _model->remove(_lastIndex);
}






/*!
 * Called when the set copy action is triggered. This changes its object's tree 
 * view's default drop action to copy. 
 */
void MetadataDialog::setCopyTriggered()
{
   // Set the default drop action to copy and change the checked status of the relevant actions.
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
   // Set the default drop action to move and change the checked status of the relevant actions.
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
   // Apply user metadata changes and signal this dialog to close with accept.
   applyClicked();
   accept();
}






/*!
 * Called when the apply button is clicked, applying the metadata of this dialog to 
 * its data object. This should only be done with user metadata. 
 */
void MetadataDialog::applyClicked()
{
   // Apply user metadata changes.
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
   // Make sure the given index is an image and the correct column.
   if ( _model->isImage(index) && index.column() == 2 )
   {
      // Create a new image view dialog with the given index and begin modal execution.
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
   // Determine what model index is located at the given point and save it as this object's last
   // index.
   _lastIndex = _view->indexAt(point);

   // Enable the add menu if the last index is a container and enable the remove action if the
   // last index is valid.
   _addMenu->setEnabled(_model->isContainer(_lastIndex));
   _removeAction->setEnabled(_lastIndex.isValid());

   // Begin modal execution of this object's context menu.
   _contextMenu->exec(QCursor::pos());
}






/*!
 * Create and initialize all actions for this new metadata dialog. 
 */
void MetadataDialog::createActions()
{
   // Create all actions for adding new metadata of any type.
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

   // Create the remove metadata action.
   _removeAction = new QAction(tr("&Remove"),this);
   connect(_removeAction,&QAction::triggered,this,&MetadataDialog::removeTriggered);

   // Create and initialize the set drag as copy action.
   _setCopyAction = new QAction(tr("&Copy"),this);
   _setCopyAction->setCheckable(true);
   _setCopyAction->setChecked(true);
   connect(_setCopyAction,&QAction::triggered,this,&MetadataDialog::setCopyTriggered);

   // Create and initialize the set drag as move action.
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
   // Create the custom context menu.
   _contextMenu = new QMenu(this);

   // Add an add menu to the custom context menu.
   _addMenu = _contextMenu->addMenu(tr("&Add New"));

   // Iterate through all add actions and add them to the add menu.
   for (auto action: _addActions)
   {
      _addMenu->addAction(action);
   }

   // Add the remove action to the custom context menu.
   _contextMenu->addAction(_removeAction);

   // Add a drag menu to the custom context menu and then add the set copy and set move actions
   // to the new drag menu.
   QMenu* dragMenu {_contextMenu->addMenu(tr("&Drag Action"))};
   dragMenu->addAction(_setCopyAction);
   dragMenu->addAction(_setMoveAction);
}
