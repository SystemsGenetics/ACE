#include "emetaobject.h"
#include "emetadata.h"
#include "edebug.h"






/*!
 * Assign this mapping to the one given. The map will not be copied until a
 * write operation is performed.
 *
 * @param object Map that this object will be assigned to.
 *
 * @return Reference to this object.
 */
EMetaObject& EMetaObject::operator=(const EMetaObject& object)
{
   // Add the debug header.
   EDEBUG_FUNC(this,&object);

   // Assign this object to the given mapping.
   QMap<QString,EMetadata>::operator=(object);

   // Return reference to this object.
   return *this;
}






/*!
 * Returns reference to metadata object with the given key. If no metadata
 * object is mapped to the given key a new null metadata object is created,
 * mapped with the given key, and returned as the reference.
 *
 * @param key Key of requested metadata object within the mapping of objects.
 *
 * @return Reference to metadata object.
 */
EMetadata& EMetaObject::operator[](const QString& key)
{
   EDEBUG_FUNC(this,key);
   return QMap<QString,EMetadata>::operator[](key);
}






/*!
 * Returns copy of metadata object with the given key. If no metadata object is
 * mapped to the given key a new null metadata object is created and returned.
 *
 * @param key Key of requested metadata object within the mapping of objects.
 *
 * @return Copy of metadata object.
 */
EMetadata EMetaObject::operator[](const QString& key) const
{
   EDEBUG_FUNC(this,key);
   return QMap<QString,EMetadata>::operator[](key);
}






/*!
 * Tests if the mapping is empty or not.
 *
 * @return Returns true of the mapping is empty else returns false.
 */
bool EMetaObject::isEmpty() const
{
   EDEBUG_FUNC(this);
   return QMap<QString,EMetadata>::isEmpty();
}






/*!
 * Gets the size of the mapping.
 *
 * @return Size of the map.
 */
int EMetaObject::size() const
{
   EDEBUG_FUNC(this);
   return QMap<QString,EMetadata>::size();
}






/*!
 * Returns iterator to first metadata item in the mapping.
 *
 * @return First metadata iterator.
 */
QMap<QString,EMetadata>::iterator EMetaObject::begin()
{
   EDEBUG_FUNC(this);
   return QMap<QString,EMetadata>::begin();
}






/*!
 * Returns constant iterator to first metadata object in the mapping.
 *
 * @return First metadata constant iterator.
 */
QMap<QString,EMetadata>::const_iterator EMetaObject::begin() const
{
   EDEBUG_FUNC(this);
   return QMap<QString,EMetadata>::begin();
}






/*!
 * Returns constant iterator to first metadata object in the mapping.
 *
 * @return First metadata constant iterator.
 */
QMap<QString,EMetadata>::const_iterator EMetaObject::cbegin() const
{
   EDEBUG_FUNC(this);
   return QMap<QString,EMetadata>::cbegin();
}






/*!
 * Returns end of mapping iterator.
 *
 * @return End of map iterator.
 */
QMap<QString,EMetadata>::iterator EMetaObject::end()
{
   EDEBUG_FUNC(this);
   return QMap<QString,EMetadata>::end();
}






/*!
 * Returns end of mapping constant iterator.
 *
 * @return End of map constant iterator.
 */
QMap<QString,EMetadata>::const_iterator EMetaObject::end() const
{
   EDEBUG_FUNC(this);
   return QMap<QString,EMetadata>::end();
}






/*!
 * Returns end of mapping constant iterator.
 *
 * @return End of map constant iterator.
 */
QMap<QString,EMetadata>::const_iterator EMetaObject::cend() const
{
   EDEBUG_FUNC(this);
   return QMap<QString,EMetadata>::cend();
}






/*!
 * Returns copy of metadata object with given key within the mapping of metadata
 * objects. If the key does not exist a new null metadata value will be created
 * and returned.
 *
 * @param key Key of requested metadata object within the mapping of objects.
 *
 * @return Copy of metadata object with given key.
 */
EMetadata EMetaObject::at(const QString& key) const
{
   EDEBUG_FUNC(this,key);
   return QMap<QString,EMetadata>::operator[](key);
}






/*!
 * Inserts a new metadata value to the mapping with the given key. If a metadata
 * value already exists with the given key it will be overwritten with the new
 * given value.
 *
 * @param key Key value where new metadata value will be mapped to.
 *
 * @param value The new metadata value to be inserted into the mapping.
 */
void EMetaObject::insert(const QString& key, const EMetadata& value)
{
   EDEBUG_FUNC(this,key,&value);
   QMap<QString,EMetadata>::operator[](key) = value;
}






/*!
 * Deletes all metadata values from the map making it an empty mapping.
 */
void EMetaObject::clear()
{
   EDEBUG_FUNC(this);
   QMap<QString,EMetadata>::clear();
}

