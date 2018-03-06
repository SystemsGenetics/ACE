#include "ace_metadatamodel_node.h"
#include "exception.h"



using namespace std;
using namespace Ace;






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
{}






/*!
 * Constructs a new node object as a copy of the given object. 
 *
 * @param object The other node object that is copied to this one. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Iterate through the given object's internal array, creating a new node 
 *    that is a copy of the node pointed to from the given object's array, and 
 *    save each new pointer to this node's array. 
 *
 * 2. Iterate through the given object's internal map, creating a new node that 
 *    is a copy of the node pointed to from the given object's map, and save 
 *    each new pointer to this node's mapping with the same key as the given 
 *    object's map. 
 */
MetadataModel::Node::Node(const Node& object):
   QObject(),
   _meta(object._meta)
{
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
 * Tests if this node is either an array or an object. 
 *
 * @return Returns true if this is an array or object, else returns false. 
 */
bool MetadataModel::Node::isContainer() const
{
   return _meta.type() == EMetadata::Array || _meta.type() == EMetadata::Object;
}






/*!
 * Tests if this node is an array. 
 *
 * @return Returns true of this is an array, else returns false. 
 */
bool MetadataModel::Node::isArray() const
{
   return _meta.type() == EMetadata::Array;
}






/*!
 * Tests of this node is an object. 
 *
 * @return Returns true of this is an object, else returns false. 
 */
bool MetadataModel::Node::isObject() const
{
   return _meta.type() == EMetadata::Object;
}






/*!
 * Returns the number of children this node contains. This works for any 
 * metadata type. If this node is not a container type then 0 is always 
 * returned. 
 *
 * @return Number of children, if any, this node contains. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this node is an array type then return this node's array size, else 
 *    proceed to the next step. 
 *
 * 2. If this node is an object type then return this node's map size, else 
 *    proceed to the next step. 
 *
 * 3. Safely assume this node is not a container so return 0. 
 */
int MetadataModel::Node::size() const
{
   if ( isArray() )
   {
      return _array.size();
   }
   else if ( isObject() )
   {
      return _map.size();
   }
   else
   {
      return 0;
   }
}






/*!
 * Returns the key or name for this node in relation to its parent. If its 
 * parent is an array the index is returned as a string. 
 *
 * @return The key or index number of this node. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Get a pointer to this node's parent, casting it is a node pointer itself. 
 *
 * 2. Make sure the casting of this node's parent to a node pointer itself 
 *    worked. If it failed throw an exception about the error. 
 *
 * 3. If the parent is an array type then find where this node is indexed within 
 *    the parent's array and return the index number as a string, else proceed 
 *    to the next step. 
 *
 * 4. If the parent is an object type then find the key in the parent's map 
 *    where this node is indexed and return it, else proceed to the next step. 
 *
 * 5. If this step is reached a logical error has been reached because any 
 *    node's parent must be an array or object. Throw an exception about the 
 *    error. 
 */
QString MetadataModel::Node::key() const
{
   const Node* parent_ {qobject_cast<const Node*>(parent())};
   if ( !parent_ )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logical Error"));
      e.setDetails(tr("Node's parent is not a node itself when it should be."));
      throw e;
   }
   if ( parent_->isArray() )
   {
      return QString::number(parent_->indexOf(this));
   }
   else if ( parent_->isObject() )
   {
      int index {parent_->indexOf(this)};
      if ( index != -1 )
      {
         return parent_->_map.keys().at(index);
      }
      return QString();
   }
   else
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logical Error"));
      e.setDetails(tr("Node's parent is not a container type when it should be."));
      throw e;
   }
}






/*!
 * Returns the metadata type for this node as a string. 
 *
 * @return Metadata type of this node. 
 */
QString MetadataModel::Node::type() const
{
   return EMetadata::typeName(_meta.type());
}






/*!
 * Returns the metadata value of this node if it is not a container type. If it 
 * is a container type a null Qt variant is returned. 
 *
 * @return Value of this node's metadata, if any. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this node is not a container type simply return the metadata value, 
 *    else proceed to the next step. 
 *
 * 2. If this node is a byte array type then return a string informing the user 
 *    this is an image, else proceed to the next step. 
 *
 * 3. If this node is an array type then return a string reporting the number of 
 *    nodes this node's array contains, else proceed to the next step. 
 *
 * 4. If this node is an object type then return a string reporting the number 
 *    of nodes this node's map contains, else proceed to the next step. 
 *
 * 5. If this step is reached then the node's metadata type must be null so 
 *    return a string stating that fact. 
 */
QVariant MetadataModel::Node::value() const
{
   switch (_meta.type())
   {
   case EMetadata::Bool:
      return _meta.toBool();
   case EMetadata::Double:
      return _meta.toDouble();
   case EMetadata::String:
      return _meta.toString();
   case EMetadata::Bytes:
      return QString("IMAGE");
   case EMetadata::Array:
      return QString::number(_array.size()).append(" items");
   case EMetadata::Object:
      return QString::number(_map.size()).append(" items");
   default:
      return QString("NULL");
   }
}






/*!
 * Sets the metadata value of this node if it is not a container type. If it is 
 * a container or null type then this will do nothing and return immediately. 
 *
 * @param value New value this node's metadata will be set to if it is not a 
 *              container type. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Set this node's metadata to the given value depending on its type. If it 
 *    is a container or null type then do nothing. 
 */
void MetadataModel::Node::setValue(const QVariant& value)
{
   switch (_meta.type())
   {
   case EMetadata::Bool:
      _meta.toBool() = value.toBool();
      break;
   case EMetadata::Double:
      _meta.toDouble() = value.toDouble();
      break;
   case EMetadata::String:
      _meta.toString() = value.toString();
      break;
   case EMetadata::Bytes:
      _meta.toBytes() = value.toByteArray();
      break;
   default:
      break;
   }
}






/*!
 * Returns first constant iterator to node pointer in this node's internal array 
 * of pointers. If this node is not an array type this list will always be 
 * empty. 
 *
 * @return First node pointer constant iterator in array. 
 */
QList<MetadataModel::Node*>::const_iterator MetadataModel::Node::arrayBegin() const
{
   return _array.begin();
}






/*!
 * Returns end of list constant iterator in this node's internal array of 
 * pointers. If this node is not an array type this list will always be empty. 
 *
 * @return End of list constant iterator of array. 
 */
QList<MetadataModel::Node*>::const_iterator MetadataModel::Node::arrayEnd() const
{
   return _array.end();
}






/*!
 * Returns first constant iterator to node pointer in this node's internal map 
 * of pointers. If this node is not an object type this list will always be 
 * empty. 
 *
 * @return First node pointer constant iterator in map. 
 */
QMap<QString,MetadataModel::Node*>::const_iterator MetadataModel::Node::objectBegin() const
{
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
   return _map.end();
}






/*!
 * Finds what index the given node pointer exists within this node if at all. If 
 * this node is not a container or the pointer is not matched to any of its 
 * children then -1 is returned. 
 *
 * @param pointer The node pointer that will be matched against any children 
 *                this node contains. 
 *
 * @return Index where match to node pointer is found or -1 if no match is 
 *         found. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If this node is not a container then return -1, else go to next step. 
 *
 * 2. Initialize the return variable to -1. 
 *
 * 3. If this node is an object type then get the list of node pointers from its 
 *    internal mapping, else it is an array type so make a direct copy of its 
 *    internal array. 
 *
 * 4. Go through the list of copied node pointers until a match is found or the 
 *    end of the list is reached. If a match is found then set the return 
 *    variable to the index where it was found. 
 *
 * 5. Return the return variable. 
 */
int MetadataModel::Node::indexOf(const Node* pointer) const
{
   if ( !isContainer() )
   {
      return -1;
   }
   int ret {-1};
   QList<Node*> list;
   if ( isObject() )
   {
      list = _map.values();
   }
   else
   {
      list = _array;
   }
   for (int i = 0; i < list.size() ;++i)
   {
      if ( list.at(i) == pointer )
      {
         ret = i;
         break;
      }
   }
   return ret;
}






/*!
 * Tests whether this node's internal mapping of node pointers already contains 
 * the given key. If this node is not an object type false is always returned. 
 *
 * @param key The given key to be tested if it already exists within this node's 
 *            mapping. 
 *
 * @return Returns true if the given key already exists, else returns false. 
 */
bool MetadataModel::Node::contains(const QString& key) const
{
   return _map.contains(key);
}






/*!
 * This will return the index a new node would be inserted into this node's 
 * internal map with the given key if this is an object type. If this node is 
 * not an object then -1 is returned. 
 *
 * @param key The given key to test for indexing position within the map. 
 *
 * @return Index where the given key would be inserted. 
 */
int MetadataModel::Node::getFutureIndex(const QString& key) const
{
   return std::distance(_map.begin(),_map.lowerBound(key));
}






/*!
 * Creates a copy of this node's child at the given index. If the given index 
 * does not exist or this node is not a container type then a null pointer is 
 * returned. 
 *
 * @param index Index of child node that is copied. 
 *
 * @return Pointer to copy of child node or null pointer if no such child 
 *         exists. 
 *
 *
 * Steps of Operation: 
 *
 * 1. If the index is within range and this node is a container type then get 
 *    the node pointer contained within the given index from this node's 
 *    internal array or map depending on its container type. 
 *
 * 2. If a node pointer was retrieved from the previous step create and return a 
 *    new node that is a copy of that node pointed to, else return a null 
 *    pointer. 
 */
std::unique_ptr<MetadataModel::Node> MetadataModel::Node::copy(int index)
{
   Node* object {nullptr};
   if ( index > 0 )
   {
      if ( isArray() && index < _array.size() )
      {
         object = _array.at(index);
      }
      else if ( isObject() && index < _map.size() )
      {
         object = _map.values().at(index);
      }
   }
   if ( object )
   {
      return unique_ptr<Node>(new Node(*object));
   }
   else
   {
      return nullptr;
   }
}






/*!
 * Cuts this node's child at the given index, returning its pointer and removing 
 * it form this node's list of children. If the given index does not exist or 
 * this node is not a container type then a null pointer is returned. 
 *
 * @param index Index of child node that is cut. 
 *
 * @return Pointer to child node removed or null pointer if no such child 
 *         exists. 
 */
std::unique_ptr<MetadataModel::Node> MetadataModel::Node::cut(int index)
{}






/*!
 * Inserts a new node with the given pointer into this node's internal array at 
 * the given index if this node is an array type. If this node is not an array 
 * type this does nothing. If the index is less than 0 the new node is prepended 
 * to the array. If the index is greater than or equal to its size it is 
 * appended to the array. 
 *
 * @param index Index where the new node is inserted. 
 *
 * @param node Pointer to the new node that is inserted. 
 */
void MetadataModel::Node::insertArray(int index, std::unique_ptr<Node>&& node)
{}






/*!
 * Inserts a new node with the given pointer into this node's internal mapping 
 * with the given key. If the key already exists the node pointer is overwritten 
 * with the new node pointer, freeing the old node's memory. If this node is not 
 * an object type this does nothing. 
 *
 * @param key The key where the new node that is inserted to in the map. 
 *
 * @param node The pointer to the new node that is inserted into the map. 
 */
void MetadataModel::Node::insertObject(const QString& key, std::unique_ptr<Node>&& node)
{}






/*!
 * Removes and deletes this node's child with the given index. This will work if 
 * the node is an array or an object type. If this node is not a container type 
 * or the index is out of range this does nothing. 
 *
 * @param index Index of the child node to be removed and deleted. 
 */
void MetadataModel::Node::remove(int index)
{}
