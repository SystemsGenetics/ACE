#ifndef ACE_METADATAMODEL_NODE_H
#define ACE_METADATAMODEL_NODE_H
#include <memory>
#include <QList>
#include <QMap>
#include <QVariant>
#include "ace_metadatamodel.h"
#include "emetadata.h"



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
   class MetadataModel::Node : public QObject
   {
      Q_OBJECT
   public:
      explicit Node(EMetadata::Type type = EMetadata::Null, QObject* parent = nullptr);
      Node(const Node& object);
      bool isContainer() const;
      bool isArray() const;
      bool isObject() const;
      int size() const;
      QString key() const;
      QString type() const;
      QVariant value() const;
      void setValue(const QVariant& value);
      QList<MetadataModel::Node*>::const_iterator arrayBegin() const;
      QList<MetadataModel::Node*>::const_iterator arrayEnd() const;
      QMap<QString,MetadataModel::Node*>::const_iterator objectBegin() const;
      QMap<QString,MetadataModel::Node*>::const_iterator objectEnd() const;
      int indexOf(const Node* pointer) const;
      bool contains(const QString& key) const;
      int getFutureIndex(const QString& key) const;
      std::unique_ptr<MetadataModel::Node> copy(int index);
      std::unique_ptr<MetadataModel::Node> cut(int index);
      void insertArray(int index, std::unique_ptr<Node>&& node);
      void insertObject(const QString& key, std::unique_ptr<Node>&& node);
      void remove(int index);
   private:
      /*!
       * Internal array of node pointers that is used to contain this node's children 
       * if it is an array type. If this node is not an array type this will always be 
       * empty. 
       */
      QList<Node*> _array;
      /*!
       * Internal mapping of node pointers that is used to contain this node's 
       * children if it is an object type. If this node is not an object type this 
       * will always be empty. 
       */
      QMap<QString,Node*> _map;
      /*!
       * Metadata object used to store the metadata type of this node. It is also used 
       * to store the value of basic types that are not arrays or objects. It is NOT 
       * used to store the children of arrays or objects. 
       */
      EMetadata _meta;
   };
}



#endif
