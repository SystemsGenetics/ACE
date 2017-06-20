#include "metadatamodel.h"
#include "metadata.h"
#include "exception.h"






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
         e.setType(InvalidParent);
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
   //
}






bool Ace::MetadataModel::removeRows(int row, int count, const QModelIndex& parent)
{
   //
}






bool Ace::MetadataModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
   //
}






QStringList Ace::MetadataModel::mimeTypes() const
{
}






Qt::DropActions Ace::MetadataModel::supportedDropActions() const
{
}






QMimeData *Ace::MetadataModel::mimeData(const QModelIndexList& indexes) const
{
}






bool Ace::MetadataModel::canDropMimeData(const QMimeData* data, Qt::DropAction action, int row
                                         , int column, const QModelIndex& parent) const
{
}






bool Ace::MetadataModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row
                                      , int column, const QModelIndex& parent)
{
}






void Ace::MetadataModel::setRoot(Metadata* root) noexcept
{
}
