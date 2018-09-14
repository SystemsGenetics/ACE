#include "ace_metadatamodel_node.h"
#include "eexception.h"
#include "edebug.h"



using namespace std;
using namespace Ace;
//






/*!
 * Constructs a new node object with the given qt object as its parent, if any. 
 *
 * @param type The metadata type of this new node. 
 *
 * @param parent The parent of this new node, if any. 
 */
MetadataModel::Node::Node(EMetadata::Type type, QObject* parent):
   QObject(parent),
   _meta(type)
{
   EDEBUG_FUNC(this,type,parent)
}






/*!
 * Constructs a new node object with the given metadata value and the given qt 
 * object as its parent, if any. If the metadata given is an array or object then 
 * the metadata values contained within are not copied. This is because nodes store 
 * their values in an entirely different manner than the metadata class. 
 *
 * @param meta The metadata value of this new node. 
 *
 * @param parent The parent of this new node, if any. 
 */
MetadataModel::Node::Node(const EMetadata& meta, QObject* parent):
   QObject(parent)
{
   EDEBUG_FUNC(this,meta,parent)

   if ( meta.isArray() || meta.isObject() )
   {
      _meta = EMetadata(meta.type());
   }
   else
   {
      _meta = meta;
   }
}






/*!
 * Constructs a new node object as a copy of the given object. 
 *
 * @param object The other node object that is copied to this one. 
 */
MetadataModel::Node::Node(const Node& object):
   QObject(),
   _meta(object._meta)
{
   EDEBUG_FUNC(this,&object)

   // Iterate through the given object's internal array, creating a new node that is 
   // a copy of the node pointed to from the given object's array, and save each new 
   // pointer to this node's array. 
   for (auto pointer : qAsConst(object._array))
   {
      Node* child {new Node(*pointer)};
      child->setParent(this);
      _array << new Node(*pointer);
   }
   for (auto i = object._map.cbegin(); i != object._map.cend() ;++i)
   {
      Node* child {new Node(**i)};
      child->setParent(this);
      _map.insert(i.key(),child);
   }
}






/*!
 * Tests if this node is a bytes type. 
 *
 * @return Returns true of this node is a bytes type else returns false. 
 */
bool MetadataModel::Node::isBytes() const
{
   EDEBUG_FUNC(this)

   return _meta.isBytes();
}






/*!
 * Tests if this node's metadata is editable as a string. 
 *
 * @return Returns true if this node is editable else returns false. 
 */
bool MetadataModel::Node::isEditable() const
{
   EDEBUG_FUNC(this)

   return _meta.isBool() || _meta.isDouble() || _meta.isString();
}






/*!
 * Tests if this node is either an array or an object. 
 *
 * @return Returns true if this is an array or object, else returns false. 
 */
bool MetadataModel::Node::isContainer() const
{
   EDEBUG_FUNC(this)

   return _meta.type() == EMetadata::Array || _meta.type() == EMetadata::Object;
}






/*!
 * Tests if this node is an array. 
 *
 * @return Returns true of this is an array, else returns false. 
 */
bool MetadataModel::Node::isArray() const
{
   EDEBUG_FUNC(this)

   return _meta.type() == EMetadata::Array;
}






/*!
 * Tests of this node is an object. 
 *
 * @return Returns true of this is an object, else returns false. 
 */
bool MetadataModel::Node::isObject() const
{
   EDEBUG_FUNC(this)

   return _meta.type() == EMetadata::Object;
}






/*!
 * Returns the number of children this node contains. This works for any metadata 
 * type. If this node is not a container type then 0 is always returned. 
 *
 * @return Number of children, if any, this node contains. 
 */
int MetadataModel::Node::size() const
{
   EDEBUG_FUNC(this)

   // If this node is an array type then return this node's array size, else proceed 
   // to the next step. 
   if ( isArray() )
   {
      return _array.size();
   }

   // If this node is an object type then return this node's map size, else proceed 
   // to the next step. 
   else if ( isObject() )
   {
      return _map.size();
   }

   // Safely assume this node is not a container so return 0. 
   else
   {
      return 0;
   }
}






/*!
 * Returns this node's parent. If this is the root node then null is returned. 
 *
 * @return Pointer to this node's parent unless this node is root. 
 */
MetadataModel::Node* MetadataModel::Node::parent() const
{
   EDEBUG_FUNC(this)

   return qobject_cast<Node*>(QObject::parent());
}






/*!
 * Returns the key or name for this node in relation to its parent. If its parent 
 * is an array the index is returned as a string. 
 *
 * @return The key or index number of this node. 
 */
QString MetadataModel::Node::key() const
{
   EDEBUG_FUNC(this)

   // Get a pointer to this node's parent, casting it is a node pointer itself. 
   const Node* parent_ {qobject_cast<const Node*>(parent())};

   // Make sure the casting of this node's parent to a node pointer itself worked. If 
   // it failed throw an exception about the error. 
   if ( !parent_ )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logical Error"));
      e.setDetails(tr("Node's parent is not a node itself when it should be."));
      throw e;
   }

   // If the parent is an array type then find where this node is indexed within the 
   // parent's array and return the index number as a string, else proceed to the 
   // next step. 
   if ( parent_->isArray() )
   {
      return QString::number(parent_->indexOf(this));
   }

   // If the parent is an object type then find the key in the parent's map where 
   // this node is indexed and return it, else proceed to the next step. 
   else if ( parent_->isObject() )
   {
      int index {parent_->indexOf(this)};
      if ( index != -1 )
      {
         return parent_->_map.keys().at(index);
      }
      return QString();
   }

   // If this step is reached a logical error has been reached because any node's 
   // parent must be an array or object. Throw an exception about the error. 
   else
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logical Error"));
      e.setDetails(tr("Node's parent is not a container type when it should be."));
      throw e;
   }
}






/*!
 * This sets the key of this node to a new value. If this node's parent is not an 
 * object type or the key already exists then false is returned. 
 *
 * @param newKey The new key for this node. 
 *
 * @return Returns true on success or false on failure. 
 */
bool MetadataModel::Node::setKey(const QString& newKey)
{
   EDEBUG_FUNC(this,newKey)

   // Get pointer to this node's parent. 
   Node* parent_ {parent()};

   // If this node has no parent, this node's parent is not an object, or this node's 
   // parent already contains a node with the new key then return false for failure. 
   // Else go to the next step. 
   if ( !parent_ || !parent_->isObject() || parent_->contains(newKey) )
   {
      return false;
   }

   // Remove the old key this node was contained in its parent and add it back with 
   // the new key. 
   QMap<QString,Node*>& map_ {parent_->_map};
   map_.remove(map_.keys().at(parent_->indexOf(this)));
   map_.insert(newKey,this);

   // Return true for success. 
   return true;
}






/*!
 * Returns the metadata type for this node as a string. 
 *
 * @return Metadata type of this node. 
 */
QString MetadataModel::Node::type() const
{
   EDEBUG_FUNC(this)

   return EMetadata::typeName(_meta.type());
}






/*!
 * Returns the byte array of this node. If this node is not a bytes type then an 
 * empty byte array is returned. 
 *
 * @return Byte array of this node, if any. 
 */
QByteArray MetadataModel::Node::bytes() const
{
   EDEBUG_FUNC(this)

   // Initialize return byte array variable to an empty array. 
   QByteArray ret;

   // If this node's metadata is a byte array set return variable to said byte array. 
   if ( _meta.isBytes() )
   {
      ret = _meta.toBytes();
   }

   // Return the return byte array variable. 
   return ret;
}






/*!
 * This returns the metadata value for this node. 
 *
 * @return This node's metadata value. 
 */
EMetadata MetadataModel::Node::meta() const
{
   EDEBUG_FUNC(this)

   return _meta;
}






/*!
 * Returns the metadata value of this node if it is not a container or bytes type. 
 * If this node is a container type then a string reporting the number of nodes it 
 * holds is returned. If this node is a bytes type then a string informing the user 
 * this is an image is returned. 
 *
 * @return Value of this node's metadata, or information about what it contains, or 
 *         a plain string stating it is a bytes(image) type. 
 */
QVariant MetadataModel::Node::value() const
{
   EDEBUG_FUNC(this)

   // If this node is not a container or bytes type simply return the metadata value, 
   // else proceed to the next step. 
   switch (_meta.type())
   {
   case EMetadata::Bool:
      return _meta.toBool();
   case EMetadata::Double:
      return _meta.toDouble();
   case EMetadata::String:
      return _meta.toString();

   // If this node is a bytes type then return a plain string informing the user this 
   // is a byte array, else go to the next step. 
   case EMetadata::Bytes:
      return tr("IMAGE");

   // If this node is an array type then return a string reporting the number of 
   // nodes this node's array contains, else proceed to the next step. 
   case EMetadata::Array:
      return tr("%1 items").arg(QString::number(_array.size()));

   // If this node is an object type then return a string reporting the number of 
   // nodes this node's map contains, else proceed to the next step. 
   case EMetadata::Object:
      return tr("%1 items").arg(QString::number(_map.size()));

   // If this step is reached then the node's metadata type must be null so return an 
   // empty qt variant. 
   default:
      return QVariant();
   }
}






/*!
 * Sets the metadata value of this node if it is not a container type. If it is a 
 * container or null type then this will do nothing and return immediately. 
 *
 * @param value New value this node's metadata will be set to if it is not a 
 *              container type. 
 *
 * @return Returns true if value was successfully set else returns false if no 
 *         value was set. 
 */
bool MetadataModel::Node::setValue(const QVariant& value)
{
   EDEBUG_FUNC(this,value)

   // If this node is not a container type then set its value and return true, else 
   // do nothing and return false. 
   switch (_meta.type())
   {
   case EMetadata::Bool:
      _meta.toBool() = value.toBool();
      return true;
   case EMetadata::Double:
      _meta.toDouble() = value.toDouble();
      return true;
   case EMetadata::String:
      _meta.toString() = value.toString();
      return true;
   case EMetadata::Bytes:
      _meta.toBytes() = value.toByteArray();
      return true;
   default:
      return false;
   }
}






/*!
 * Returns first constant iterator to node pointer in this node's internal array of 
 * pointers. If this node is not an array type this list will always be empty. 
 *
 * @return First node pointer constant iterator in array. 
 */
QList<MetadataModel::Node*>::const_iterator MetadataModel::Node::arrayBegin() const
{
   EDEBUG_FUNC(this)

   return _array.begin();
}






/*!
 * Returns end of list constant iterator in this node's internal array of pointers. 
 * If this node is not an array type this list will always be empty. 
 *
 * @return End of list constant iterator of array. 
 */
QList<MetadataModel::Node*>::const_iterator MetadataModel::Node::arrayEnd() const
{
   EDEBUG_FUNC(this)

   return _array.end();
}






/*!
 * Returns first constant iterator to node pointer in this node's internal map of 
 * pointers. If this node is not an object type this list will always be empty. 
 *
 * @return First node pointer constant iterator in map. 
 */
QMap<QString,MetadataModel::Node*>::const_iterator MetadataModel::Node::objectBegin() const
{
   EDEBUG_FUNC(this)

   return _map.begin();
}






/*!
 * Returns end of list constant iterator in this node's internal mapping of 
 * pointers. If this node is not an object type this list will always be empty. 
 *
 * @return End of list constant iterator of map. 
 */
QMap<QString,MetadataModel::Node*>::const_iterator MetadataModel::Node::objectEnd() const
{
   EDEBUG_FUNC(this)

   return _map.end();
}






/*!
 * Returns the node pointer contains in this node's internal container, map or 
 * array, with the given index. If this node is not a container type or the index 
 * is out of range then null is returned. 
 *
 * @param index The index of the requested node pointer. 
 *
 * @return Node pointer with given index, if any. 
 */
MetadataModel::Node* MetadataModel::Node::get(int index) const
{
   EDEBUG_FUNC(this,index)

   // Initialize return pointer variable to null. 
   Node* ret {nullptr};

   // If the index is within range and this node is a container type then set the 
   // return pointer variable to the node pointer contained with the given index. 
   if ( index < 0 )
   {
      return ret;
   }
   if ( isArray() && index < _array.size() )
   {
      ret = _array.at(index);
   }
   else if ( isObject() && index < _map.size() )
   {
      ret = _map.values().at(index);
   }

   // Return the return pointer variable. 
   return ret;
}






/*!
 * Finds what index the given node pointer exists within this node if at all. If 
 * this node is not a container or the pointer is not matched to any of its 
 * children then -1 is returned. 
 *
 * @param pointer The node pointer that will be matched against any children this 
 *                node contains. 
 *
 * @return Index where match to node pointer is found or -1 if no match is found. 
 */
int MetadataModel::Node::indexOf(const Node* pointer) const
{
   EDEBUG_FUNC(this,pointer)

   // If this node is not a container then return -1, else go to next step. 
   if ( !isContainer() )
   {
      return -1;
   }

   // Initialize the return variable to -1. 
   int ret {-1};

   // If this node is an object type then get the list of node pointers from its 
   // internal mapping, else it is an array type so make a direct copy of its 
   // internal array. 
   QList<Node*> list;
   if ( isObject() )
   {
      list = _map.values();
   }
   else
   {
      list = _array;
   }

   // Go through the list of copied node pointers until a match is found or the end 
   // of the list is reached. If a match is found then set the return variable to the 
   // index where it was found. 
   for (int i = 0; i < list.size() ;++i)
   {
      if ( list.at(i) == pointer )
      {
         ret = i;
         break;
      }
   }

   // Return the return variable. 
   return ret;
}






/*!
 * Tests whether this node's internal mapping of node pointers already contains the 
 * given key. If this node is not an object type false is always returned. 
 *
 * @param key The given key to be tested if it already exists within this node's 
 *            mapping. 
 *
 * @return Returns true if the given key already exists, else returns false. 
 */
bool MetadataModel::Node::contains(const QString& key) const
{
   EDEBUG_FUNC(this,key)

   return _map.contains(key);
}






/*!
 * This will return the index a new node would be inserted into this node's 
 * internal map with the given key if this is an object type. If this node is not 
 * an object then -1 is returned. 
 *
 * @param key The given key to test for indexing position within the map. 
 *
 * @return Index where the given key would be inserted. 
 */
int MetadataModel::Node::getFutureIndex(const QString& key) const
{
   EDEBUG_FUNC(this,key)

   return std::distance(_map.begin(),_map.lowerBound(key));
}






/*!
 * Creates a copy of this node's child at the given index. If the given index does 
 * not exist or this node is not a container type then a null pointer is returned. 
 *
 * @param index Index of child node that is copied. 
 *
 * @return Pointer to copy of child node or null pointer if no such child exists. 
 */
std::unique_ptr<MetadataModel::Node> MetadataModel::Node::copy(int index)
{
   EDEBUG_FUNC(this,index)

   // Initialize the return smart pointer to null. 
   unique_ptr<Node> ret;

   // If the index is within range and this node is a container type then make a new 
   // node, setting its pointer to the return smart pointer, that is a copy of the 
   // node pointed to at the given index from this node's internal array or map 
   // depending on its container type. 
   if ( index > 0 )
   {
      if ( isArray() && index < _array.size() )
      {
         ret.reset(new Node(*_array.at(index)));
      }
      else if ( isObject() && index < _map.size() )
      {
         ret.reset(new Node(*_map.values().at(index)));
      }
   }

   // Return the return smart pointer which is null or set to a copy of the node 
   // pointed to at the given index. 
   return ret;
}






/*!
 * Cuts this node's child at the given index, returning its pointer and removing it 
 * form this node's list of children. If the given index does not exist or this 
 * node is not a container type then a null pointer is returned. 
 *
 * @param index Index of child node that is cut. 
 *
 * @return Pointer to child node removed or null pointer if no such child exists. 
 */
std::unique_ptr<MetadataModel::Node> MetadataModel::Node::cut(int index)
{
   EDEBUG_FUNC(this,index)

   // Initialize the return smart pointer to null. 
   unique_ptr<Node> ret;

   // If the index is within range and this node is a container type then take the 
   // node pointer from this node's array or map, depending on what type this node 
   // is, and set the return smart pointer to the taken pointer. 
   if ( index < 0 )
   {
      return ret;
   }
   if ( isArray() && index < _array.size() )
   {
      ret.reset(_array.takeAt(index));
   }
   else if ( isObject() && index < _map.size() )
   {
      ret.reset(_map.take(_map.keys().at(index)));
   }

   // If the return smart pointer is not null then set the pointed to node's parent 
   // to null. 
   if ( ret )
   {
      ret->setParent(nullptr);
   }

   // Return the return smart pointer which is null or set to the node pointer found 
   // at the given index. 
   return ret;
}






/*!
 * Inserts a new node with the given pointer into this node's internal array at the 
 * given index if this node is an array type. If this node is not an array type 
 * this does nothing but free then given node pointer. If the index is less than 0 
 * the new node is prepended to the array. If the index is greater than or equal to 
 * its size it is appended to the array. 
 *
 * @param index Index where the new node is inserted. 
 *
 * @param node Pointer to the new node that is inserted. 
 */
void MetadataModel::Node::insertArray(int index, std::unique_ptr<Node>&& node)
{
   EDEBUG_FUNC(this,index,node.get())

   // If this node is an array type then insert the given node pointer into this 
   // node's internal array setting the new node's parent to this node, else delete 
   // the node pointed to. 
   if ( isArray() )
   {
      node->setParent(this);
      _array.insert(index,node.release());
   }
   else
   {
      node.reset();
   }
}






/*!
 * Inserts a new node with the given pointer into this node's internal mapping with 
 * the given key. If the key already exists the node pointer is overwritten with 
 * the new node pointer, freeing the old node's memory. If this node is not an 
 * object type this does nothing but free the given node pointer. 
 *
 * @param key The key where the new node that is inserted to in the map. 
 *
 * @param node The pointer to the new node that is inserted into the map. 
 */
void MetadataModel::Node::insertObject(const QString& key, std::unique_ptr<Node>&& node)
{
   EDEBUG_FUNC(this,key,node.get())

   // If this node is an object type then go to the next step, else delete the node 
   // pointed to and exit. 
   if ( isObject() )
   {
      // If a node pointer already exists with the given key delete the node pointed to. 
      // Insert the new node pointer with the given key setting the new node's parent to 
      // this node. 
      node->setParent(this);
      delete _map[key];
      _map[key] = node.release();
   }
   else
   {
      node.reset();
   }
}






/*!
 * Removes and deletes this node's child with the given index. This will work if 
 * the node is an array or an object type. If this node is not a container type or 
 * the index is out of range this does nothing. 
 *
 * @param index Index of the child node to be removed and deleted. 
 */
void MetadataModel::Node::remove(int index)
{
   EDEBUG_FUNC(this,index)

   // If the index is greater then 0 go to the next step, else exit the function. 
   if ( index > 0 )
   {
      // If the node is an array type and the index is less than the size of the array 
      // then go to the next step, else go to step 4. 
      if ( isArray() && index < _array.size() )
      {
         // Remove the node pointer from this node's internal array at the given index and 
         // delete the node it points to. Exit the function. 
         delete _array.takeAt(index);
      }

      // If this node is an object and the index is less than the size of the map then 
      // go to the next step, else exit the function. 
      else if ( isObject() && index < _map.size() )
      {
         // Remove the node pointer from this node's internal mapping at the given index 
         // and delete the node it points to. 
         delete _map.take(_map.keys().at(index));
      }
   }
}
