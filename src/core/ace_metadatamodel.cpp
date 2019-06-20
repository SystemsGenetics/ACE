#include "ace_metadatamodel.h"
#include "ace_metadatamodelnode.h"
#include "emetadata.h"
#include "emetaarray.h"
#include "emetaobject.h"
#include "eexception.h"
#include "edebug.h"



namespace Ace
{



/*!
 * This stores the string identifier for the custom mime type used for this
 * model to have drag and drop functionality.
 */
const char* MetadataModel::_mimeType {"ace/metadatamodel.node.pointer"};






/*!
 * This implements the interface that informs Qt what mime types this model
 * supports for drag and drop support.
 *
 * @return List of mime types this model supports for drag and drop.
 */
QStringList MetadataModel::mimeTypes() const
{
   EDEBUG_FUNC(this);
   return QStringList() << _mimeType;
}






/*!
 * This implements the interface that informs Qt what drag and drop actions this
 * model supports. The drag and drop actions this model supports is moving and
 * copying.
 *
 * @return Supported drag and drop actions for this model.
 */
Qt::DropActions MetadataModel::supportedDropActions() const
{
   EDEBUG_FUNC(this);
   return Qt::CopyAction|Qt::MoveAction;
}






/*!
 * This implements the interface that informs any view what the headers should
 * be for columns and/or rows of the view. This returns the header data for the
 * given section, orientation, and role. This model has no headers for the row
 * and has headers for the first three columns denoting "key", "type", and
 * "value".
 *
 * @param section The section for the requested header data. This is the indent,
 *                starting at 0, for the given orientation of vertical or
 *                horizontal.
 *
 * @param orientation The orientation for the requested header data.
 *
 * @param role The role for the requested header data.
 *
 * @return The header data for the given section, orientation, and role.
 */
QVariant MetadataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
   // Add the debug header.
   EDEBUG_FUNC(this,section,orientation,role);

   // If this is not the display role or the orientation is not vertical then return
   // a null variant else go to the next step.
   if ( role != Qt::DisplayRole || orientation != Qt::Horizontal )
   {
      return QVariant();
   }

   // If the section is between 0 and 2 return the correct header name else return a
   // null variant.
   switch (section)
   {
   case 0: return tr("Key");
   case 1: return tr("Type");
   case 2: return tr("Value");
   default: return QVariant();
   }
}






/*!
 * This implements the interface that creates a new index from the given row,
 * column, and parent index.
 *
 * @param row Row for the requested index.
 *
 * @param column Column for the requested index.
 *
 * @param parent Parent index for the requested index.
 *
 * @return Index for the given row, column, and parent index.
 */
QModelIndex MetadataModel::index(int row, int column, const QModelIndex& parent) const
{
   // Add the debug header.
   EDEBUG_FUNC(this,row,column,parent);

   // Get the node pointer of the given parent index.
   MetadataModelNode* parent_ {pointer(parent)};

   // If the parent node is not a container then throw an exception, else go to the
   // next step.
   if ( !parent_->isContainer() )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Metadata Model"));
      e.setDetails(tr("Discovered metadata parent that is not array or object."));
      throw e;
   }

   // If the row is out of range for the parent node's list of children nodes then
   // return an invalid index, else go to the next step.
   if ( row < 0 || row >= parent_->size() )
   {
      return QModelIndex();
   }

   // Return a new index for the given row, column, and parent.
   return createIndex(row,column,parent_->get(row));
}






/*!
 * This implements the interface that returns the parent index of the given
 * index for this model.
 *
 * @param child The child index of the parent index to be found.
 *
 * @return Parent index of the given index.
 */
QModelIndex MetadataModel::parent(const QModelIndex& child) const
{
   // Add the debug header.
   EDEBUG_FUNC(this,child);

   // Get node pointer to parent of the given child index. If the parent is null then
   // return an invalid index else go to the next step.
   MetadataModelNode* parent {pointer(child)->parent()};
   if ( !parent )
   {
      return QModelIndex();
   }

   // Get node pointer to grandparent of the given child index. If the grandparent is
   // null then return an invalid index else go to the next step.
   MetadataModelNode* grandParent {parent->parent()};
   if ( !grandParent )
   {
      return QModelIndex();
   }

   // Return a new index of the parent of the given index.
   return createIndex(grandParent->indexOf(parent),0,parent);
}






/*!
 * This implements the interface that returns the flags for a given index of
 * this mode. The flags inform anyone what can and cannot be done to the given
 * index.
 *
 * @param index The index whose flags are returned.
 *
 * @return Flags for the given index.
 */
Qt::ItemFlags MetadataModel::flags(const QModelIndex& index) const
{
   // Add the debug header.
   EDEBUG_FUNC(this,index);

   // Create return flag variable with default flags all indexes contain.
   Qt::ItemFlags ret {Qt::ItemIsSelectable|Qt::ItemIsEnabled};
   MetadataModelNode* node {pointer(index)};
   if ( _readOnly )
   {
      return ret;
   }

   // If the index is valid then add drag enabled to the return flags.
   if ( index.isValid() )
   {
      ret |= Qt::ItemIsDragEnabled;
   }

   // If the node of the given index is a container then add drop enabled to the
   // return flags.
   if ( node->isContainer() )
   {
      ret |= Qt::ItemIsDropEnabled;
   }
   switch (index.column())
   {
   // If the given index is the first column, the node of the index has a parent, and
   // the node's parent is an object type, then add is editable to the return flags.
   case 0:
      if ( node->parent() && node->parent()->isObject() )
      {
         ret |= Qt::ItemIsEditable;
      }
      break;

   // If the index is the third column and the node of the given index is editable
   // then add is editable to the return flags.
   case 2:
      if ( node->isEditable() )
      {
         ret |= Qt::ItemIsEditable;
      }
      break;
   }
   return ret;
}






/*!
 * This implements the interface that returns the number of rows a given index
 * contains. This number resolves to the number of nodes contained in the node
 * of the given index. If the node is not a container 0 is returned.
 *
 * @param parent The index whose number of rows are returned.
 *
 * @return The number of rows for the given index.
 */
int MetadataModel::rowCount(const QModelIndex& parent) const
{
   EDEBUG_FUNC(this,parent);
   if ( !_root ) return 0;
   return pointer(parent)->size();
}






/*!
 * This implements the interface that returns the number of columns a given
 * index contains. For this model the number of columns is always 3.
 *
 * @param parent This is not used because the number of columns is static.
 *
 * @return The number of columns which is always 3 for this model.
 */
int MetadataModel::columnCount(const QModelIndex& parent) const
{
   EDEBUG_FUNC(this,parent);
   Q_UNUSED(parent)
   return 3;
}






/*!
 * This implements the interface that creates a new qt mime data object when a
 * drag and drop action is initiated. This model creates a custom mime data type
 * just for this model which consists of a node pointer that will be copied or
 * moved. This model does not support multiple indexes being drag and dropped at
 * once so only the first index in the list of indexes will ever be used.
 *
 * @param indexes List of indexes that is being requested for drag and drop
 *                action. This model only uses the first index in the list.
 *
 * @return Pointer to new qt mime data object.
 */
QMimeData* MetadataModel::mimeData(const QModelIndexList& indexes) const
{
   // Add the debug header.
   EDEBUG_FUNC(this,&indexes);

   // Initialize byte array and stream for writing to it.
   QByteArray bytes;
   QDataStream stream(&bytes,QIODevice::WriteOnly);

   // Write node pointer from the first index of the given indexes and make sure it
   // worked. If writing failed then return a null pointer else go to next step.
   stream << reinterpret_cast<quintptr>(indexes.at(0).internalPointer());
   if ( stream.status() != QDataStream::Ok )
   {
      return nullptr;
   }

   // Create a new qt mime data object, then set its mime data to this model's custom
   // type with the byte array as its data, then return a pointer to the new qt mime
   // data object.
   QMimeData* data = new QMimeData;
   data->setData(_mimeType,bytes);
   return data;
}






/*!
 * This implements the interface that returns data from the given index and
 * role. This model only returns data based off the display role or custom raw
 * image data role. This model returns a string suitable for a view with the
 * display role. Only if the node of the given index is a bytes type then the
 * byte array is returned with the raw image data role.
 *
 * @param index The index whose data is being requested.
 *
 * @param role The role of the data being requested.
 *
 * @return Data of given index and role.
 */
QVariant MetadataModel::data(const QModelIndex& index, int role) const
{
   // Add the debug header.
   EDEBUG_FUNC(this,index,role);

   // Get the node pointer for the given index.
   MetadataModelNode* node {pointer(index)};

   // If the given role is raw image data and the node is a bytes type then return
   // the byte array of the given index.
   if ( role == RawImageData && node->isBytes() )
   {
      return node->bytes();
   }

   // Else if the given role is not display then return a null variant.
   else if ( role != Qt::DisplayRole )
   {
      return QVariant();
   }

   // Return the correct information based off the column of the given index. If the
   // given column is unknown then return a null variant.
   switch (index.column())
   {
   case 0:
      return node->key();
   case 1:
      return node->type();
   case 2:
      return node->value();
   default:
      return QVariant();
   }
}






/*!
 * This implements the interface that sets the data of a given index with the
 * given role. The only role this model implements is the edit role. All other
 * roles are ignored and false is returned.
 *
 * @param index The index whose data will be changed.
 *
 * @param value The new value for the given index's data.
 *
 * @param role The role of the data being set.
 *
 * @return Returns true if the data of the given index was successfully changed,
 *         else returns false.
 */
bool MetadataModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
   // Add the debug header.
   EDEBUG_FUNC(this,index,value,role);

   // Initialize return value to false.
   bool ret {false};

   // If the given role is the edit role go to the next step, else go to the last
   // step.
   if ( role == Qt::EditRole )
   {
      switch (index.column())
      {
      // If the given index is the first column then set its key, setting the return
      // value to the result of setting the key.
      case 0:
         ret = setKey(index,value.toString());
         break;

      // If the given index is the third column then set its value to the given one,
      // setting the return variable to the result of that operation. If setting a new
      // value was successful then emit a data changed signal for the model.
      case 2:
         if ( (ret = pointer(index)->setValue(value)) )
         {
            dataChanged(index,index);
         }
         break;
      }
   }

   // Return the return variable.
   return ret;
}






/*!
 * This implements the interface that handles the end of a drag and drop
 * operation where the drop has occurred. This model only implements the copy
 * and move actions.
 *
 * @param data Pointer to qt mime data object that was created when the drag was
 *             initiated.
 *
 * @param action The drag and drop action being requested.
 *
 * @param row The row where the drop occurred.
 *
 * @param column The column where the drop occurred.
 *
 * @param parent The parent index where the drop occurred.
 *
 * @return Returns true on success of the drag and drop action or false on
 *         failure.
 */
bool MetadataModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
   // Add the debug header.
   EDEBUG_FUNC(this,data,action,row,column,parent);

   // If the drag and drop action is not copy or move then return false, else go to
   // the next step.
   Q_UNUSED(column)
   if ( action != Qt::CopyAction && action != Qt::MoveAction )
   {
      return false;
   }

   // Extract the node pointer from the qt mime data object. If reading the node
   // pointer in fails then return false, else go the the next step.
   QByteArray bytes = data->data(_mimeType);
   QDataStream stream(bytes);
   quintptr pointer;
   stream >> pointer;
   MetadataModelNode* node = reinterpret_cast<MetadataModelNode*>(pointer);
   if ( stream.status() != QDataStream::Ok )
   {
      return false;
   }

   // Either copy a new node from the node pointer or take it from its current
   // location in the model, depending on if the action is copy or move,
   // respectively.
   std::unique_ptr<MetadataModelNode> newNode;
   switch (action)
   {
   case Qt::CopyAction:
      newNode.reset(new MetadataModelNode(*node));
      break;
   case Qt::MoveAction:
      newNode = take(node);
      break;
   default:
      break;
   }

   // Insert the new or taken node into the model at the new location where the drop
   // occurred.
   insert(parent,row,std::move(newNode));

   // Return true for success.
   return true;
}






/*!
 * This constructs a new metadata model with the given parent, if any.
 *
 * @param parent The parent for this new model.
 */
MetadataModel::MetadataModel(QObject* parent)
   :
   QAbstractItemModel(parent),
   _root(new MetadataModelNode(EMetadata::Object,this))
{
   EDEBUG_FUNC(this,parent);
}






/*!
 * Tests if the given index is a bytes type which stores the data of an image.
 *
 * @param index The given index to test if it is an image.
 *
 * @return Returns true if the given index is an image else returns false.
 */
bool MetadataModel::isImage(const QModelIndex& index) const
{
   EDEBUG_FUNC(this,index);
   return pointer(index)->isBytes();
}






/*!
 * Tests if the given index is a container type, either an array or object type.
 *
 * @param index The given index to test if it is a container type.
 *
 * @return Returns true if the given index is a container type else returns
 *         false.
 */
bool MetadataModel::isContainer(const QModelIndex& index) const
{
   EDEBUG_FUNC(this,index);
   return pointer(index)->isContainer();
}






/*!
 * Returns the read only state of this model.
 *
 * @return True if this model is read only or false otherwise.
 */
bool MetadataModel::readOnly() const
{
   EDEBUG_FUNC(this);
   return _readOnly;
}






/*!
 * Returns this model's data tree in the form of metadata objects. The root
 * metadata object returned is an object type that is the root of the tree for
 * the model.
 *
 * @return Root metadata object of this model's data tree.
 */
EMetadata MetadataModel::meta() const
{
   EDEBUG_FUNC(this);
   return buildMeta(_root);
}






/*!
 * Inserts a new empty node with the given metadata type into the given parent.
 * If the parent is a map type a new key is generated else if the parent is an
 * array type the new node is prepended to the array.
 *
 *
 *
 * @return Returns true on success else returns false.
 */
bool MetadataModel::insert(const QModelIndex& parent, EMetadata::Type type)
{
   EDEBUG_FUNC(this,parent,type);
   return insert(parent,0,std::unique_ptr<MetadataModelNode>(new MetadataModelNode(type)));
}






/*!
 * Removes the given index from this model.
 *
 * @param index The given index that is deleted.
 *
 * @return Returns true on success or false on failure.
 */
bool MetadataModel::remove(const QModelIndex& index)
{
   // Add the debug header.
   EDEBUG_FUNC(this,index);

   // If the given index is not valid then return false, else go to the next step.
   if ( !index.isValid() )
   {
      return false;
   }

   // Get the parent index of the given index.
   QModelIndex parent {MetadataModel::parent(index)};
   int row {index.row()};

   // Remove the given index from the parent's container.
   beginRemoveRows(parent,row,row);
   pointer(parent)->remove(row);
   endRemoveRows();

   // Return true on success.
   return true;
}






/*!
 * This sets its model's data to the given metadata value. Any data stored
 * within the model is removed and overwritten. The metadata object given must
 * be an object type or this will throw an exception.
 *
 * @param newRoot The given metadata root object that will be copied into this
 *                model's data.
 */
void MetadataModel::setMeta(const EMetadata& newRoot)
{
   // Add the debug header.
   EDEBUG_FUNC(this,&newRoot);

   // If the given metadata object is not an object type then throw an exception,
   // else go to the next step.
   if ( !newRoot.isObject() )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Metadata Model"));
      e.setDetails(tr("Cannot set root of model to metadata that is not an object type."));
      throw e;
   }

   // Delete the current node data on this model if any exists.
   beginResetModel();
   delete _root;

   // Copy a new node data tree from the given metadata object and set it is this
   // mode's new data tree.
   _root = buildNode(newRoot).release();
   _root->setParent(this);
   endResetModel();
}






/*!
 * Set the read only state of this model.
 *
 * @param state New boolean value for this model's read only state.
 */
void MetadataModel::setReadOnly(bool state)
{
   // Add the debug header.
   EDEBUG_FUNC(this,state);

   // Update this model's read only state with the new value, signaling this model is
   // resetting.
   beginResetModel();
   _readOnly = state;
   endResetModel();
}






/*!
 * Returns the node pointer of the given index. If the given index is invalid
 * the root node of this model is returned.
 *
 * @param index The index of the requested node pointer.
 *
 * @return The node pointer of the given index.
 */
MetadataModelNode* MetadataModel::pointer(const QModelIndex& index) const
{
   // Add the debug header.
   EDEBUG_FUNC(this,index);

   // If the index is valid extract its internal pointer and return it as a node
   // pointer, else return this model's root node pointer.
   MetadataModelNode* ret;
   if ( index.isValid() )
   {
      ret = reinterpret_cast<MetadataModelNode*>(index.internalPointer());
   }
   else
   {
      ret = _root;
   }
   return ret;
}






/*!
 * Sets the key of the given index to the new given value.
 *
 * @param index The given index whose key is changed.
 *
 * @param newKey The new key value.
 *
 * @return Returns true and success or false on failure.
 */
bool MetadataModel::setKey(const QModelIndex& index, const QString& newKey)
{
   // Add the debug header.
   EDEBUG_FUNC(this,index,newKey);

   // If the parent of the given index is not object type and/or already contains the
   // new key then return false, else go to the next step.
   bool ret {false};

   // Find out if changing the key of the given index will change its row position
   // within it's parent's map. Then set the key of the given index to the new given
   // key, notifying the model the position of the index has changed if that is the
   // case. Then return true on success.
   MetadataModelNode* node {pointer(index)};
   MetadataModelNode* parent {node->parent()};
   if ( !parent->contains(newKey) )
   {
      int oldRow {parent->indexOf(node)};
      int newRow {parent->getFutureIndex(newKey)};
      bool doesMove { oldRow != newRow && (oldRow + 1) != newRow };
      if ( doesMove )
      {
         QModelIndex parent {MetadataModel::parent(index)};
         beginMoveRows(parent,oldRow,oldRow,parent,newRow);
      }
      node->setKey(newKey);
      if ( doesMove )
      {
         endMoveRows();
      }
      ret = true;
   }
   return ret;
}






/*!
 * Take ownership of the given node pointer, removing it from this model. If the
 * given node is this model's root node nothing is done and a null pointer is
 * returned because it is impossible to remove the root node.
 *
 * @param node Node pointer to take ownership.
 *
 * @return Node pointer.
 */
std::unique_ptr<MetadataModelNode> MetadataModel::take(MetadataModelNode* node)
{
   // Add the debug header.
   EDEBUG_FUNC(this,node);

   // If the node does not have a parent because it is the root node then return a
   // null pointer, else go to the next step.
   MetadataModelNode* parent {node->parent()};
   if ( !parent )
   {
      return nullptr;
   }

   // Get the parent index of the given node and its row within the parent node. Then
   // remove the given node from its parent, informing the model of the removal. Then
   // return ownership of the given node pointer.
   QModelIndex parent_;
   if ( parent->parent() )
   {
      parent_ = createIndex(parent->parent()->indexOf(parent),0,parent);
   }
   int row {parent->indexOf(node)};
   beginRemoveRows(parent_,row,row);
   std::unique_ptr<MetadataModelNode> ret {parent->cut(row)};
   endRemoveRows();
   return ret;
}






/*!
 * This inserts a new node into the model into the given parent index at the
 * given row. If the parent node is an object type the given row is ignored
 * because those lists are ordered by the map. If the parent node is an array
 * the row is used; if the row is less than 0 the new node is prepended to the
 * parent else if it is beyond the end of the list the new node is appended. If
 * this fails at inserting the new node it is deleted.
 *
 * @param parent The parent where the new node will be inserted into.
 *
 * @param row The row where the new node will be inserted within the parent.
 *            Ignored if the parent is an object type.
 *
 * @param node The new node that will be inserted if successful or deleted if
 *             insertion failed.
 *
 * @return Returns true on successful insertion else returns false.
 */
bool MetadataModel::insert(const QModelIndex& parent, int row, std::unique_ptr<MetadataModelNode>&& node)
{
   // Add the debug header.
   EDEBUG_FUNC(this,parent,row,node.get());

   // Get node pointer of given parent index.
   MetadataModelNode* parent_ {pointer(parent)};

   // If the node parent is an object type then go to the next step, else go to step
   // 5.
   if ( parent_->isObject() )
   {
      // Generate a new key that does not exist in the parent node's mapping of nodes.
      QString newKey {tr("unnamed")};
      while ( parent_->contains(newKey) )
      {
         newKey.append("_");
      }

      // Insert the new node into the parent node's map with the new generated key.
      // Return true for success.
      int row {parent_->getFutureIndex(newKey)};
      beginInsertRows(parent,row,row);
      parent_->insertObject(newKey,std::move(node));
      endInsertRows();
   }

   // If the node parent is an array type then go to the next step, else go to the
   // last step.
   else if ( parent_->isArray() )
   {
      // If the row is out of range, change it to 0 or the end of the list depending on
      // it being less than 0 or greater than the size, respectively.
      if ( row > parent_->size() )
      {
         row = parent_->size();
      }
      else if ( row < 0 )
      {
         row = 0;
      }

      // Insert the new node into the parent node's array with the given and possibly
      // modified row. Return true for success.
      beginInsertRows(parent,row,row);
      parent_->insertArray(row,std::move(node));
      endInsertRows();
   }

   // The parent is not an object or array so delete the new node and return false
   // for failure.
   else
   {
      node.reset();
      return false;
   }
   return true;
}






/*!
 * Recursively builds metadata value from the given pointed to node. The entire
 * tree is copied, recursively building all children of arrays and objects.
 *
 * @param node The node pointer whose node is copied.
 *
 * @return Copy of the given pointed to node.
 */
EMetadata MetadataModel::buildMeta(const MetadataModelNode* node) const
{
   // Add the debug header.
   EDEBUG_FUNC(this,node);

   // Create a new metadata variable that is a copy of the node's metadata value. If
   // the metadata variable is a container type then recursively copy all children
   // nodes into the metadata variable with the same keys if it is an object type.
   // Then return the new metadata variable.
   EMetadata ret {node->meta()};
   if ( ret.isArray() )
   {
      for (auto i = node->arrayBegin(); i != node->arrayEnd() ;++i)
      {
         ret.toArray().append(buildMeta(*i));
      }
   }
   else if ( ret.isObject() )
   {
      for (auto i = node->objectBegin(); i != node->objectEnd() ;++i)
      {
         ret.toObject().insert(i.key(),buildMeta(*i));
      }
   }
   return ret;
}






/*!
 * Builds node value from the given metadata value, recursively building all
 * children of arrays and objects.
 *
 * @param meta The metadata value that is copied.
 *
 * @return Copy of the given metadata value.
 */
std::unique_ptr<MetadataModelNode> MetadataModel::buildNode(const EMetadata& meta)
{
   // Add the debug header.
   EDEBUG_FUNC(this,&meta);

   // Create a new node variable, storing its pointer, setting it's metadata type as
   // the given metadata value's type. If the given metadata type is a container then
   // recursively copy all children metadata into the new node with the same keys if
   // it is an object type. Then return the node pointer of the new node.
   std::unique_ptr<MetadataModelNode> ret {new MetadataModelNode(meta)};
   if ( meta.isArray() )
   {
      for (auto child : qAsConst(meta.toArray()))
      {
         ret->insertArray(ret->size(),buildNode(child));
      }
   }
   else if ( meta.isObject() )
   {
      for (auto i = meta.toObject().cbegin(); i != meta.toObject().cend() ;++i)
      {
         ret->insertObject(i.key(),buildNode(*i));
      }
   }
   return ret;
}

}
