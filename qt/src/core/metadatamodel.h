#ifndef METADATAMODEL_H
#define METADATAMODEL_H
#include <QtCore>



namespace Ace
{
class Metadata;



class MetadataModel : public QAbstractItemModel
{
   Q_OBJECT
public:
   enum Errors
   {
      InvalidParent = 0
      ,InvalidIndex
      ,InvalidVariant
   };
   explicit MetadataModel(QObject* parent = nullptr) noexcept;
   MetadataModel(const MetadataModel&) = delete;
   MetadataModel(MetadataModel&&) = delete;
   QVariant headerData(int section, Qt::Orientation orientation, int role) const override final;
   QModelIndex index(int row, int column, const QModelIndex &parent) const override final;
   QModelIndex parent(const QModelIndex &child) const override final;
   int rowCount(const QModelIndex& parent) const override final;
   int columnCount(const QModelIndex& parent) const override final;
   QVariant data(const QModelIndex& index, int role) const override final;
   Qt::ItemFlags flags(const QModelIndex& index) const override final;
   bool removeRows(int row, int count, const QModelIndex& parent) override final;
   bool setData(const QModelIndex& index, const QVariant& value, int role) override final;
   QStringList mimeTypes() const override final;
   Qt::DropActions supportedDropActions() const override final;
   QMimeData* mimeData(const QModelIndexList& indexes) const override final;
   bool canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column
                        , const QModelIndex& parent) const;
   bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column
                     , const QModelIndex& parent) override final;
   void setRoot(Metadata* root) noexcept;
private:
   Metadata* _root {nullptr};
};
}



#endif
