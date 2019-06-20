#ifndef ACE_METADATAMODELNODE_H
#define ACE_METADATAMODELNODE_H
#include <memory>
#include <QList>
#include <QMap>
#include <QVariant>
#include "emetadata.h"



/*!
 * This contains all private classes used internally by the ACE library and
 * should never be accessed by a developer using this library.
 */
namespace Ace
{
   /*!
    * This class represents a single metadata node within the metadata model. The
    * reason this class must exist as a wrapper around the metadata class is
    * because the model must work with pointers for arrays and objects while the
    * metadata classes do not use pointers. The node can be any possible metadata
    * class. If it is an array or object it uses its own custom qt containers that
    * hold pointers instead of metadata arrays or objects. The nodes are also Qt
    * Objects to allow for easy memory cleanup.
    */
   class MetadataModelNode : public QObject
   {
      Q_OBJECT
   public:
      explicit MetadataModelNode(EMetadata::Type type = EMetadata::Null, QObject* parent = nullptr);
      explicit MetadataModelNode(const EMetadata& meta, QObject* parent = nullptr);
      explicit MetadataModelNode(const MetadataModelNode& object);
   public:
      bool isBytes() const;
      bool isEditable() const;
      bool isContainer() const;
      bool isArray() const;
      bool isObject() const;
      int size() const;
      MetadataModelNode* parent() const;
      QString key() const;
      bool setKey(const QString& newKey);
      QString type() const;
      QByteArray bytes() const;
      EMetadata meta() const;
      QVariant value() const;
      bool setValue(const QVariant& value);
      QList<MetadataModelNode*>::const_iterator arrayBegin() const;
      QList<MetadataModelNode*>::const_iterator arrayEnd() const;
      QMap<QString,MetadataModelNode*>::const_iterator objectBegin() const;
      QMap<QString,MetadataModelNode*>::const_iterator objectEnd() const;
      MetadataModelNode* get(int index) const;
      int indexOf(const MetadataModelNode* pointer) const;
      bool contains(const QString& key) const;
      int getFutureIndex(const QString& key) const;
      std::unique_ptr<MetadataModelNode> copy(int index);
      std::unique_ptr<MetadataModelNode> cut(int index);
      void insertArray(int index, std::unique_ptr<MetadataModelNode>&& node);
      void insertObject(const QString& key, std::unique_ptr<MetadataModelNode>&& node);
      void remove(int index);
   private:
      /*!
       * Internal array of node pointers that is used to contain this node's children
       * if it is an array type. If this node is not an array type this will always be
       * empty.
       */
      QList<MetadataModelNode*> _array;
      /*!
       * Internal mapping of node pointers that is used to contain this node's
       * children if it is an object type. If this node is not an object type this
       * will always be empty.
       */
      QMap<QString,MetadataModelNode*> _map;
      /*!
       * Metadata object used to store the metadata type of this node. It is also used
       * to store the value of basic types that are not arrays or objects. It is NOT
       * used to store the children of arrays or objects.
       */
      EMetadata _meta;
   };
}

#endif
