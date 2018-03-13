#include "ace_metadatamodel.h"
#include "ace_metadatamodel_node.h"
#include "emetadata.h"
#include "exception.h"



using namespace std;
using namespace Ace;
//



/*!
 */
const char* MetadataModel::_mimeType {"ace/metadatamodel.node.pointer"};






/*!
 *
 * @param parent  
 */
MetadataModel::MetadataModel(QObject* parent):
   QAbstractItemModel(parent),
   _root(new Node(EMetadata::Object,this))
{}






/*!
 */
QStringList MetadataModel::mimeTypes() const
{
   return QStringList() << _mimeType;
}






/*!
 */
Qt::DropActions MetadataModel::supportedDropActions() const
{
   return Qt::CopyAction|Qt::MoveAction;
}






/*!
 *
 * @param section  
 *
 * @param orientation  
 *
 * @param role  
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
 *
 * @param row  
 *
 * @param column  
 *
 * @param parent  
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
      parent_->insertObject(newKey,unique_ptr<Node>(new Node(type)));
      endInsertRows();
   }
   else if ( parent_->isArray() )
   {
      beginInsertRows(parent,0,0);
      parent_->insertArray(0,unique_ptr<Node>(new Node(type)));
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
void MetadataModel::insert(const QModelIndex& parent, int row, std::unique_ptr<Node>&& node)
{}






/*!
 *
 * @param node  
 */
EMetadata MetadataModel::buildMeta(const Node* node) const
{}






/*!
 *
 * @param meta  
 */
std::unique_ptr<MetadataModel::Node> MetadataModel::buildNode(const EMetadata& meta)
{}
