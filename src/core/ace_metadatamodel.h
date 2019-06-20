#ifndef ACE_METADATAMODEL_H
#define ACE_METADATAMODEL_H
#include <memory>
#include <QAbstractItemModel>
#include "emetadata.h"
#include "ace.h"



/*!
 * This contains all private classes used internally by the ACE library and
 * should never be accessed by a developer using this library.
 */
namespace Ace
{
   /*!
    * This provides a Qt item model for a metadata structure. The root of the
    * metadata structure must be an object type. This model is primarily used for
    * the GUI dialog to edit the metadata of a data object. It can however be used
    * as a general model for any purpose. The model can insert or remove a row, and
    * drag and drop a single row that either copies or pastes the row. It consists
    * of three columns. The first column is the name, the second is the type, and
    * the last column is the value.
    */
   class MetadataModel : public QAbstractItemModel
   {
      Q_OBJECT
   public:
      /*!
       * This contains custom data roles for this model.
       */
      enum Roles
      {
         /*!
          * This is for the data role of getting the byte array of a bytes metadata type.
          * The normal display role for a bytes type will just return an "IMAGE" string.
          */
         RawImageData = 10000
      };
   public:
      virtual QStringList mimeTypes() const override final;
      virtual Qt::DropActions supportedDropActions() const override final;
      virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override final;
      virtual QModelIndex index(int row, int column, const QModelIndex& parent) const override final;
      virtual QModelIndex parent(const QModelIndex& child) const override final;
      virtual Qt::ItemFlags flags(const QModelIndex& index) const override final;
      virtual int rowCount(const QModelIndex& parent) const override final;
      virtual int columnCount(const QModelIndex& parent) const override final;
      virtual QMimeData* mimeData(const QModelIndexList& indexes) const override final;
      virtual QVariant data(const QModelIndex& index, int role) const override final;
      virtual bool setData(const QModelIndex& index, const QVariant& value, int role) override final;
      virtual bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) override final;
   public:
      explicit MetadataModel(QObject* parent = nullptr);
   public:
      bool isImage(const QModelIndex& index) const;
      bool isContainer(const QModelIndex& index) const;
      bool readOnly() const;
      EMetadata meta() const;
      bool insert(const QModelIndex& parent, EMetadata::Type type);
      bool remove(const QModelIndex& index);
      void setMeta(const EMetadata& newRoot);
      void setReadOnly(bool state);
   private:
      /*!
       * This stores the string identifier for the custom mime type used for this
       * model to have drag and drop functionality.
       */
      static const char* _mimeType;
   private:
      MetadataModelNode* pointer(const QModelIndex& index) const;
      bool setKey(const QModelIndex& index, const QString& newKey);
      std::unique_ptr<MetadataModelNode> take(MetadataModelNode* node);
      bool insert(const QModelIndex& parent, int row, std::unique_ptr<MetadataModelNode>&& node);
      EMetadata buildMeta(const MetadataModelNode* node) const;
      std::unique_ptr<MetadataModelNode> buildNode(const EMetadata& meta);
   private:
      /*!
       * This stores the root node of the metadata this model contains. The node must
       * be the an object type.
       */
      MetadataModelNode* _root;
      /*!
       * True if this model is read only and cannot be edited or false otherwise.
       */
      bool _readOnly {false};
   };
}

#endif
