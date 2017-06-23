#include "metadatamodel.h"
#include "metadata.h"
#include "exception.h"





/*
Ace::MetadataModel::MetadataModel(QObject *parent) noexcept:
   QAbstractItemModel(parent)
{}






QVariant Ace::MetadataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
   // make sure it is display role and horizontal orientation
   if ( role != Qt::DisplayRole && orientation != Qt::Vertical )
   {
      return QVariant();
   }

   // return name of column
   switch (section)
   {
   case 0:
      return QVariant(QString(tr("Name")));
   case 1:
      return QVariant(QString(tr("Type")));
   case 2:
      return QVariant(QString(tr("Value")));
   default:
      // if invalid column return nothing
      return QVariant();
   }
}






QModelIndex Ace::MetadataModel::index(int row, int column, const QModelIndex& parent) const
{
   // get pointer of parent index
   Metadata* metaParent;
   if ( parent.isValid() )
   {
      metaParent = reinterpret_cast<Metadata*>(parent.internalPointer());
   }
   else
   {
      metaParent = _root;
   }

   // get pointer of child
   Metadata* child;
   if ( metaParent->isArray() )
   {
      child = metaParent->toArray()[row];
   }
   else if ( metaParent->isObject() )
   {
      child = metaParent->toObject().at(row).second;
   }
   else
   {
      // parent is a type it should not be so report error
      E_MAKE_EXCEPTION(e);
      e.setLevel(EException::Critical);
      e.setType(InvalidParent);
      e.setTitle(tr("Metadata Model"));
      e.setDetails(tr("Discovered metadata parent that is not array or object."));
      throw e;
   }

   // return new index
   return createIndex(row,column,child);
}






QModelIndex Ace::MetadataModel::parent(const QModelIndex& child) const
{
   // get pointer of child and parent
   Metadata* metaChild = reinterpret_cast<Metadata*>(child.internalPointer());
   Metadata* parent = metaChild->getParent();

   // make sure parent and grandparent exists
   if ( !parent || !parent->getParent() )
   {
      return QModelIndex();
   }

   // get pointer to grandparent and return parent index
   Metadata* grandparent = parent->getParent();
   return createIndex(grandparent->getChildIndex(parent),child.column(),parent);
}






int Ace::MetadataModel::rowCount(const QModelIndex& parent) const
{
   // make sure there is a root metadata object
   if ( !_root )
   {
      return 0;
   }

   // get pointer of parent index
   Metadata* metaParent;
   if ( parent.isValid() )
   {
      metaParent = reinterpret_cast<Metadata*>(parent.internalPointer());
   }
   else
   {
      metaParent = _root;
   }

   // return array size if parent is array
   if ( metaParent->isArray() )
   {
      return metaParent->toArray().size();
   }

   // return object size if parent is object
   else if ( metaParent->isObject() )
   {
      return metaParent->toObject().size();
   }

   // return zero if parent is not array/object
   else
   {
      return 0;
   }
}






int Ace::MetadataModel::columnCount(const QModelIndex& parent) const
{
   Q_UNUSED(parent);
   return 3;
}






QVariant Ace::MetadataModel::data(const QModelIndex& index, int role) const
{
   // return nothing if role is not for display
   if ( role != Qt::DisplayRole )
   {
      return QVariant();
   }

   // get pointer of index and return data depending on column
   Metadata* meta = reinterpret_cast<Metadata*>(index.internalPointer());
   switch (index.column())
   {
   case 0:
   {
      // get parent and return number or key depending on parent type
      Metadata* parent = meta->getParent();
      if ( parent->isArray() )
      {
         return QVariant(index.row());
      }
      else if ( parent->isObject() )
      {
         return QVariant(parent->toObject().at(index.row()).first);
      }
      else
      {
         // parent is of type it should not be so report error
         E_MAKE_EXCEPTION(e);
         e.setLevel(EException::Critical);
         e.setType(InvalidIndex);
         e.setTitle(tr("Metadata Model"));
         e.setDetails(tr("Discovered metadata parent that is not array or object."));
         throw e;
      }
   }
   case 1:
      return QVariant(meta->getTypeName());
   case 2:
      return meta->toVariant();
   default:
      return QVariant();
   }
}






Qt::ItemFlags Ace::MetadataModel::flags(const QModelIndex& index) const
{
   // setup basic flags that always exist
   Qt::ItemFlags ret {Qt::ItemIsSelectable|Qt::ItemIsEnabled};

   // get pointer to metadata of index
   Metadata* meta;
   if ( index.isValid() )
   {
      // get pointer to valid index and add ability to drag it
      meta = reinterpret_cast<Metadata*>(index.internalPointer());
      ret |= Qt::ItemIsDragEnabled;
   }
   else
   {
      meta = _root;
   }

   // if metadata type is not array, object, or null then it is also editable
   if ( !meta->isArray() && !meta->isObject() && !meta->isNull() )
   {
      ret |= Qt::ItemIsEditable;
   }

   // if metadata type is array or object enable as drop target
   if ( meta->isArray() || meta->isObject() )
   {
      ret |= Qt::ItemIsDropEnabled;
   }

   // return flags
   return ret;
}






bool Ace::MetadataModel::removeRows(int row, int count, const QModelIndex& parent)
{
   // get pointer to metadata of parent
   Metadata* metaParent;
   if ( parent.isValid() )
   {
      metaParent = reinterpret_cast<Metadata*>(parent.internalPointer());
   }
   else
   {
      metaParent = _root;
   }

   // check if metadata type is array
   if ( metaParent->isArray() )
   {
      // get list and begin remove row opertation
      QList<Metadata*>& list {metaParent->toArray()};
      beginRemoveRows(parent,row,row+count-1);

      // remove rows then end remove row operation
      for (int i = 0; i < count ;++i)
      {
         list.removeAt(row);
      }
      endRemoveRows();
   }

   // check if metadata type is object
   else if ( metaParent->isObject() )
   {
      // get list and begin remove row opertation
      QList<QPair<QString,Metadata*>>& list {metaParent->toObject()};
      beginRemoveRows(parent,row,row+count-1);

      // remove rows then end remove row operation
      for (int i = 0; i < count ;++i)
      {
         list.removeAt(row);
      }
      endRemoveRows();
   }

   // if metadata type is neither array or object an error has occured
   else
   {
      E_MAKE_EXCEPTION(e);
      e.setLevel(EException::Critical);
      e.setType(InvalidParent);
      e.setTitle(tr("Metadata Model"));
      e.setDetails(tr("Attempting to remove rows of metadata that is not array or object."));
      throw e;
   }

   // return success of operation
   return true;
}






bool Ace::MetadataModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
   // make sure this is the edit role
   if ( role != Qt::EditRole )
   {
      return false;
   }

   // get pointer to metadata of index
   Metadata* meta = reinterpret_cast<Metadata*>(index.internalPointer());

   // set new value for metadata depending on what type it is
   if ( meta->isBool() )
   {
      meta->toBool() = value.toBool();
   }
   else if ( meta->isDouble() )
   {
      bool ok {false};
      meta->toDouble() = value.toDouble(&ok);

      // make sure floating point value was given correctly
      if ( !ok )
      {
         E_MAKE_EXCEPTION(e);
         e.setLevel(EException::Critical);
         e.setType(InvalidVariant);
         e.setTitle(tr("Metadata Model"));
         e.setDetails(tr("Could not get floating point value from qt variant class."));
         throw e;
      }
   }
   else if ( meta->isString() )
   {
      meta->toString() = value.toString();
   }

   // if this metadata is of a type that does not hold data report error
   else
   {
      E_MAKE_EXCEPTION(e);
      e.setLevel(EException::Critical);
      e.setType(InvalidIndex);
      e.setTitle(tr("Metadata Model"));
      e.setDetails(tr("Attempting to edit value of metadata that is an array or object."));
      throw e;
   }

   // return success of operation
   return true;
}






QStringList Ace::MetadataModel::mimeTypes() const
{
   return QStringList() << "application/metadata.pointer";
}






Qt::DropActions Ace::MetadataModel::supportedDropActions() const
{
   return Qt::MoveAction;
}






QMimeData* Ace::MetadataModel::mimeData(const QModelIndexList& indexes) const
{
   // if the indexes are empty return nothing
   if ( indexes.isEmpty() )
   {
      return nullptr;
   }

   // initialize byte array and data stream
   QByteArray pointers;
   QDataStream stream(&pointers,QIODevice::WriteOnly);
   pointers.reserve(sizeof(quint16)+(sizeof(quintptr)*indexes.size()));

   // write number of pointers to byte array
   quint16 size = indexes.size();
   stream << size;

   // go through list of all indexes and write metadata pointer to byte array for each one
   for (auto i = indexes.constBegin(); i != indexes.constEnd() ;++i)
   {
      const QModelIndex& index {*i};
      quintptr pointer = reinterpret_cast<quintptr>(index.internalPointer());
      stream << pointer;
   }

   // make sure all writing to byte array was successful
   if ( stream.status() != QDataStream::Ok )
   {
      return nullptr;
   }

   // create new mime data and return it with pointer byte array
   QMimeData* data {new QMimeData};
   data->setData("application/metadata.pointer",pointers);
   return data;
}






bool Ace::MetadataModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row
                                      , int column, const QModelIndex& parent)
{
   // both column and action are unused
   Q_UNUSED(column);
   Q_UNUSED(action);

   // get pointer byte array of mime data
   QByteArray pointers = data->data("application/metadata.pointer");
   QDataStream stream(pointers);

   // get number of pointers
   quint16 size;
   stream >> size;

   // generate and get list of pointers
   QList<Metadata*> metadata;
   for (int i = 0; i < size ;++i)
   {
      quintptr pointer;
      stream >> pointer;
      metadata.push_back(reinterpret_cast<Metadata*>(pointer));
   }

   // make sure stream was successful in getting all information
   if ( stream.status() != QDataStream::Ok )
   {
      return false;
   }

   // go through list of pointers and remove them from their old parents
   for (int i = 0; i < size ;++i)
   {
      Metadata* child {metadata.at(i)};
      Metadata* parent {child->getParent()};
      int row = parent->getChildIndex(child);
      beginRemoveRows(MetadataModel::parent(createIndex(row,0,child)),row,row);
      if ( parent->isArray() )
      {
         parent->toArray().removeAt(row);
      }
      else if ( parent->isObject() )
      {
         parent->toObject().removeAt(row);
      }
      else
      {
         // if parent is not an object or array report error
         E_MAKE_EXCEPTION(e);
         e.setLevel(EException::Critical);
         e.setType(InvalidParent);
         e.setTitle(tr("Metadata Model"));
         e.setDetails(tr("Attempting to move rows of metadata that is not array or object."));
         throw e;
      }
      endRemoveRows();
   }

   // if row is not specified start at beginning
   if ( row == -1 )
   {
      row = 0;
   }

   // get new parent target for dropped children
   Metadata* target;
   if ( parent.isValid() )
   {
      target = reinterpret_cast<Metadata*>(parent.internalPointer());
   }
   else
   {
      target = _root;
   }

   // go through each pointer and add to to new parent target
   beginInsertRows(parent,row,row+size-1);
   for (int i = 0; i < size ;++i)
   {
      if ( target->isArray() )
      {
         target->toArray().insert(row,metadata.at(i));
      }
      else if ( target->isObject() )
      {
         // get child pointer and list of new parent target
         Metadata* child {metadata.at(i)};
         QList<QPair<QString,Metadata*>>& list {target->toObject()};

         // make sure key of new child is unique
         int count {0};
         while ( hasKey(child->getKey(),list) )
         {
            child->setKey(child->getKey() + QString::number(count++));
         }
         list.insert(row,QPair<QString,Metadata*>(child->getKey(),child));
      }
      else
      {
         // if new parent is not array or object report error
         E_MAKE_EXCEPTION(e);
         e.setLevel(EException::Critical);
         e.setType(InvalidParent);
         e.setTitle(tr("Metadata Model"));
         e.setDetails(tr("Attempting to move rows to metadata that is not array or object."));
         throw e;
      }
   }

   // end row insertion operation and return success
   endInsertRows();
   return true;
}






void Ace::MetadataModel::setRoot(Metadata* root) noexcept
{
   // if root is not null make sure it is correct object type
   if ( root && !root->isObject() )
   {
      E_MAKE_EXCEPTION(e);
      e.setLevel(EException::Critical);
      e.setType(InvalidRoot);
      e.setTitle(tr("Metadata Model"));
      e.setDetails(tr("Attempting to root of metadata model that is not an object type."));
      throw e;
   }

   // set new root
   beginResetModel();
   _root = root;
   endResetModel();
}






bool Ace::MetadataModel::hasKey(const QString& key, QList<QPair<QString,Ace::Metadata*>>& list)
{
   // go through list and return true if key is found that matches one given
   for (auto i = list.constBegin(); i != list.constEnd() ;++i)
   {
      if ( i->first == key )
      {
         return true;
      }
   }

   // no matching key found return false
   return false;
}
*/
