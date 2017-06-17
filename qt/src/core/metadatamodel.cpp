#include "metadatamodel.h"
#include "metadata.h"






Ace::MetadataModel::MetadataModel(QObject *parent) noexcept:
   QAbstractItemModel(parent)
{}






QVariant Ace::MetadataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
   //
}






QModelIndex Ace::MetadataModel::index(int row, int column, const QModelIndex& parent) const
{
   //
}






QModelIndex Ace::MetadataModel::parent(const QModelIndex& child) const
{
   //
}






int Ace::MetadataModel::rowCount(const QModelIndex& parent) const
{
   //
}






int Ace::MetadataModel::columnCount(const QModelIndex& parent) const
{
   //
}






QVariant Ace::MetadataModel::data(const QModelIndex& index, int role) const
{
   //
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
