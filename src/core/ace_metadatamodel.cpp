#include "ace_metadatamodel.h"
#include "ace_metadatamodel_node.h"
#include "emetadata.h"
#include "emetaarray.h"
#include "emetaobject.h"
#include "exception.h"



using namespace std;
using namespace Ace;
//



/*!
 * This stores the string identifier for the custom mime type used for this 
 * model to have drag and drop functionality. 
 */
const char* MetadataModel::_mimeType {"ace/metadatamodel.node.pointer"};






/*!
 * This constructs a new metadata model with the given parent, if any. 
 *
 * @param parent The parent for this new model. 
 */
MetadataModel::MetadataModel(QObject* parent):
   QAbstractItemModel(parent),
   _root(new Node(EMetadata::Object,this))
{}






/*!
 * This implements the interface that informs Qt what mime types this model 
 * supports for drag and drop support. 
 *
 * @return List of mime types this model supports for drag and drop. 
 */
QStringList MetadataModel::mimeTypes() const
{
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
 *
 *
 * Steps of Operation: 
 *
 * 1. If this is not the display role or the orientation is not vertical then 
 *    return a null variant else go to the next step. 
 *
 * 2. If the section is between 0 and 2 return the correct header name else 
 *    return a null variant. 
 */
QVariant MetadataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
   if ( role != Qt::DisplayRole || orientation != Qt::Vertical )
   {
      return QVariant();
   }
   switch (section)
   {
   case 0: return tr("Name");
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
 *
 *
 * Steps of Operation: 
 *
 * 1. Get the node pointer of the given parent index. 
 *
 * 2. If the parent node is not a container then throw an exception, else go to 
 *    the next step. 
 *
 * 3. If the row is out of range for the parent node's list of children nodes 
 *    then return an invalid index, else go to the next step. 
 *
 * 4. Return a new index for the given row, column, and parent. 
 */
QModelIndex MetadataModel::index(int row, int column, const QModelIndex& parent) const
{
   Node* parent_ {pointer(parent)};
   if ( !parent_->isContainer() )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Metadata Model"));
      e.setDetails(tr("Discovered metadata parent that is not array or object."));
      throw e;
   }
   if ( row < 0 || row >= parent_->size() )
   {
      return QModelIndex();
   }
   return createIndex(row,column,parent_->get(row));
}






/*!
 *
 * @param child  
 */
QModelIndex MetadataModel::parent(const QModelIndex& child) const
{
   Node* parent {pointer(child)->parent()};
   if ( !parent )
   {
      return QModelIndex();
   }
   Node* grandParent {parent->parent()};
   if ( !grandParent )
   {
      return QModelIndex();
   }
   return createIndex(grandParent->indexOf(parent),0,parent);
}






/*!
 *
 * @param index  
 */
Qt::ItemFlags MetadataModel::flags(const QModelIndex& index) const
{
   Qt::ItemFlags ret {Qt::ItemIsSelectable|Qt::ItemIsEnabled};
   if ( index.isValid() )
   {
      ret |= Qt::ItemIsDragEnabled;
   }
   Node* node {pointer(index)};
   if ( node->isContainer() )
   {
      ret |= Qt::ItemIsDropEnabled;
   }
   switch (index.column())
   {
   case 0:
      if ( node->parent() && node->parent()->isObject() )
      {
         ret |= Qt::ItemIsEditable;
      }
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
 *
 * @param parent  
 */
int MetadataModel::rowCount(const QModelIndex& parent) const
{
   return pointer(parent)->size();
}






/*!
 *
 * @param parent  
 */
int MetadataModel::columnCount(const QModelIndex& parent) const
{
   Q_UNUSED(parent)
   return 3;
}






/*!
 *
 * @param indexes  
 */
QMimeData* MetadataModel::mimeData(const QModelIndexList& indexes) const
{
   QByteArray bytes;
   QDataStream stream(&bytes,QIODevice::WriteOnly);
   stream << reinterpret_cast<quintptr>(indexes.at(0).internalPointer());
   if ( stream.status() != QDataStream::Ok )
   {
      return nullptr;
   }
   QMimeData* data = new QMimeData;
   data->setData(_mimeType,bytes);
   return data;
}






/*!
 *
 * @param index  
 *
 * @param role  
 */
QVariant MetadataModel::data(const QModelIndex& index, int role) const
{
   if ( role != Qt::DisplayRole )
   {
      return QVariant();
   }
   Node* node {pointer(index)};
   if ( role == RawImageData && node->isBytes() )
   {
      return node->bytes();
   }
   return node->value();
}






/*!
 *
 * @param index  
 *
 * @param value  
 *
 * @param role  
 */
bool MetadataModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
   bool ret {false};
   if ( role == Qt::EditRole )
   {
      switch (index.column())
      {
      case 0:
         ret = setKey(index,value.toString());
      case 2:
         if ( (ret = pointer(index)->setValue(value)) )
         {
            dataChanged(index,index);
         }
      }
   }
   return ret;
}






/*!
 *
 * @param data  
 *
 * @param action  
 *
 * @param row  
 *
 * @param column  
 *
 * @param parent  
 */
bool MetadataModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
   Q_UNUSED(column)
   if ( action != Qt::CopyAction || action != Qt::MoveAction )
   {
      return false;
   }
   QByteArray bytes = data->data(_mimeType);
   QDataStream stream(bytes);
   quintptr pointer;
   stream >> pointer;
   Node* node = reinterpret_cast<Node*>(pointer);
   if ( stream.status() != QDataStream::Ok )
   {
      return false;
   }
   unique_ptr<Node> newNode;
   switch (action)
   {
   case Qt::CopyAction:
      newNode.reset(new Node(*node));
      break;
   case Qt::MoveAction:
      newNode = take(node);
      break;
   default:
      break;
   }
   insert(parent,row,std::move(newNode));
   return true;
}






/*!
 *
 * @param index  
 */
bool MetadataModel::isImage(const QModelIndex& index) const
{
   return pointer(index)->isBytes();
}






/*!
 *
 * @param index  
 */
bool MetadataModel::isContainer(const QModelIndex& index) const
{
   return pointer(index)->isContainer();
}






/*!
 *
 * @param parent  
 *
 * @param type  
 */
bool MetadataModel::insert(const QModelIndex& parent, EMetadata::Type type)
{
   return insert(parent,0,unique_ptr<Node>(new Node(type)));
}






/*!
 *
 * @param index  
 */
bool MetadataModel::remove(const QModelIndex& index)
{
   if ( !index.isValid() )
   {
      return false;
   }
   QModelIndex parent {MetadataModel::parent(index)};
   int row {index.row()};
   beginRemoveRows(parent,row,row);
   pointer(parent)->remove(row);
   endRemoveRows();
   return true;
}






/*!
 */
EMetadata MetadataModel::meta() const
{
   return buildMeta(_root);
}






/*!
 *
 * @param newRoot  
 */
void MetadataModel::setMeta(const EMetadata& newRoot)
{
   if ( !newRoot.isObject() )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Metadata Model"));
      e.setDetails(tr("Cannot set root of model to metadata that is not an object type."));
      throw e;
   }
   beginResetModel();
   delete _root;
   _root = buildNode(newRoot).release();
   _root->setParent(this);
   endResetModel();
}






/*!
 *
 * @param index  
 */
MetadataModel::Node* MetadataModel::pointer(const QModelIndex& index) const
{
   Node* ret;
   if ( index.isValid() )
   {
      ret = reinterpret_cast<Node*>(index.internalPointer());
   }
   else
   {
      ret = _root;
   }
   return ret;
}






/*!
 *
 * @param index  
 *
 * @param newKey  
 */
bool MetadataModel::setKey(const QModelIndex& index, const QString& newKey)
{
   bool ret {false};
   Node* node {pointer(index)};
   Node* parent {node->parent()};
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
 *
 * @param node  
 */
std::unique_ptr<MetadataModel::Node> MetadataModel::take(Node* node)
{
   Node* parent {node->parent()};
   if ( !parent )
   {
      return nullptr;
   }
   QModelIndex parent_;
   if ( parent->parent() )
   {
      parent_ = createIndex(parent->parent()->indexOf(parent),0,parent);
   }
   int row {parent->indexOf(node)};
   beginRemoveRows(parent_,row,row);
   unique_ptr<Node> ret {parent->cut(row)};
   endRemoveRows();
   return ret;
}






/*!
 *
 * @param parent  
 *
 * @param row  
 *
 * @param node  
 */
bool MetadataModel::insert(const QModelIndex& parent, int row, std::unique_ptr<Node>&& node)
{
   Node* parent_ {pointer(parent)};
   if ( parent_->isObject() )
   {
      QString newKey {tr("unnamed")};
      while ( parent_->contains(newKey) )
      {
         newKey.append("_");
      }
      int row {parent_->getFutureIndex(newKey)};
      beginInsertRows(parent,row,row);
      parent_->insertObject(newKey,std::move(node));
      endInsertRows();
   }
   else if ( parent_->isArray() )
   {
      if ( row >= parent_->size() )
      {
         row = parent_->size() - 1;
      }
      else if ( row < 0 )
      {
         row = 0;
      }
      beginInsertRows(parent,row,row);
      parent_->insertArray(row,std::move(node));
      endInsertRows();
   }
   else
   {
      return false;
   }
   return true;
}






/*!
 *
 * @param node  
 */
EMetadata MetadataModel::buildMeta(const Node* node) const
{
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
 *
 * @param meta  
 */
std::unique_ptr<MetadataModel::Node> MetadataModel::buildNode(const EMetadata& meta)
{
   unique_ptr<Node> ret {new Node(meta)};
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
