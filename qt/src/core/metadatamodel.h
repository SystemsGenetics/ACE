#ifndef METADATAMODEL_H
#define METADATAMODEL_H
#include <QtCore>



class EMetadata;
namespace Ace
{



class MetadataModel : public QAbstractItemModel
{
   Q_OBJECT
public:
   explicit MetadataModel(QObject* parent = nullptr) noexcept: QAbstractItemModel(parent) {}
   MetadataModel(const MetadataModel&) = delete;
   MetadataModel(MetadataModel&&) = delete;
   QVariant headerData(int section, Qt::Orientation orientation, int role) const override final;
   QModelIndex index(int row, int column, const QModelIndex &parent) const override final;
   QModelIndex parent(const QModelIndex &child) const override final;
   int rowCount(const QModelIndex& parent) const override final;
   int columnCount(const QModelIndex& parent) const override final;
   QVariant data(const QModelIndex& index, int role) const override final;
   Qt::ItemFlags flags(const QModelIndex& index) const override final;
   bool isInsertable(const QModelIndex& index) const;
   bool insertRow(EMetadata* data, const QModelIndex& parent);
   bool removeRows(int row, int count, const QModelIndex& parent) override final;
   bool setData(const QModelIndex& index, const QVariant& value, int role) override final;
   QStringList mimeTypes() const override final
      { return QStringList() << "ace/metadata.pointer"; }
   Qt::DropActions supportedDropActions() const override final
      { return Qt::CopyAction|Qt::MoveAction; }
   QMimeData* mimeData(const QModelIndexList& indexes) const override final;
   bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column
                     , const QModelIndex& parent) override final;
   void setRoot(EMetadata* root) noexcept;
private:
   EMetadata* _root {nullptr};
};
}



#endif
