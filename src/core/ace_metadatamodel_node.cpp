#include "ace_metadatamodel_node.h"



using namespace Ace;






/*!
 * Constructs a new node object with the given qt object as its parent, if any. 
 *
 * @param parent The parent of this new node, if any. 
 */
MetadataModel::Node::Node(QObject* parent)
{}






/*!
 * Tests if this node is either an array or an object. 
 *
 * @return Returns true if this is an array or object, else returns false. 
 */
bool MetadataModel::Node::isContainer() const
{}






/*!
 * Tests if this node is an array. 
 *
 * @return Returns true of this is an array, else returns false. 
 */
bool MetadataModel::Node::isArray() const
{}






/*!
 * Tests of this node is an object. 
 *
 * @return Returns true of this is an object, else returns false. 
 */
bool MetadataModel::Node::isObject() const
{}






/*!
 * Returns the number of children this node contains. This works for any 
 * metadata type. If this node is not a container type then 0 is always 
 * returned. 
 *
 * @return Number of children, if any, this node contains. 
 */
int MetadataModel::Node::size() const
{}






/*!
 * Returns the key or name for this node in relation to its parent. If its 
 * parent is an array the index is returned as a string. 
 *
 * @return The key or index number of this node. 
 */
QString MetadataModel::Node::key() const
{}






/*!
 * Returns the metadata type for this node as a string. 
 *
 * @return Metadata type of this node. 
 */
QString MetadataModel::Node::type() const
{}






/*!
 * Returns the metadata value of this node if it is not a container type. If it 
 * is a container type a null Qt variant is returned. 
 *
 * @return Value of this node's metadata, if any. 
 */
QVariant MetadataModel::Node::value() const
{}






/*!
 * Sets the metadata value of this node if it is not a container type. If it is 
 * a container type this will do nothing and return immediately. 
 *
 * @param value New value this node's metadata will be set to if it is not a 
 *              container type. 
 */
void MetadataModel::Node::setValue(const QVariant& value)
{}






/*!
 * Returns first constant iterator to node pointer in this node's internal array 
 * of pointers. If this node is not an array type this list will always be 
 * empty. 
 *
 * @return First node pointer constant iterator in array. 
 */
QList<Node*>::const_iterator MetadataModel::Node::arrayBegin() const
{}






/*!
 * Returns end of list constant iterator in this node's internal array of 
 * pointers. If this node is not an array type this list will always be empty. 
 *
 * @return End of list constant iterator of array. 
 */
QList<Node*>::const_iterator MetadataModel::Node::arrayEnd() const
{}






/*!
 * Returns first constant iterator to node pointer in this node's internal map 
 * of pointers. If this node is not an object type this list will always be 
 * empty. 
 *
 * @return First node pointer constant iterator in map. 
 */
QMap<QString,Node*>::const_iterator MetadataModel::Node::objectBegin() const
{}






/*!
 * Returns end of list constant iterator in this node's internal mapping of 
 * pointers. If this node is not an object type this list will always be empty. 
 *
 * @return End of list constant iterator of map. 
 */
QMap<QString,Node*>::const_iterator MetadataModel::Node::objectEnd() const
{}






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
 */
int MetadataModel::Node::indexOf(Node* pointer) const
{}






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
{}






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
{}






/*!
 * Creates a copy of this node's child at the given index. If the given index 
 * does not exist or this node is not a container type then a null pointer is 
 * returned. 
 *
 * @param index Index of child node that is copied. 
 *
 * @return Pointer to copy of child node or null pointer if no such child 
 *         exists. 
 */
std::unique_ptr<Node> MetadataModel::Node::copy(int index)
{}






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
std::unique_ptr<Node> MetadataModel::Node::cut(int index)
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
