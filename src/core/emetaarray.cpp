#include "emetaarray.h"
#include "emetadata.h"
#include "edebug.h"






/*!
 * Assign this array to the one given. The array will not be copied until a
 * write operation is performed.
 *
 * @param object Array that this object will be assigned to.
 *
 * @return Reference to this object.
 */
EMetaArray& EMetaArray::operator=(const EMetaArray& object)
{
   // Add the debug header.
   EDEBUG_FUNC(this,&object);

   // Assign this object to the given array.
   QList<EMetadata>::operator=(object);

   // Return reference to this object.
   return *this;
}






/*!
 * Returns reference to metadata object with the given index. If the index is
 * out of range of the array then Qt will force exit the program.
 *
 * @param index Index of requested metadata object within the array.
 *
 * @return Reference to metadata object.
 */
EMetadata& EMetaArray::operator[](int index)
{
   EDEBUG_FUNC(this,index);
   return QList<EMetadata>::operator[](index);
}






/*!
 * Returns constant reference to metadata object with the given index. If the
 * index is out of range of the array then Qt will force exit the program.
 *
 * @param index Index of requested metadata object within the array.
 *
 * @return Constant reference to metadata object.
 */
const EMetadata& EMetaArray::operator[](int index) const
{
   EDEBUG_FUNC(this,index);
   return QList<EMetadata>::operator[](index);
}






/*!
 * Appends a new metadata value to the end of the array.
 *
 * @param value The new metadata value to be appended to the end of the array.
 *
 * @return Reference to this object.
 */
EMetaArray& EMetaArray::operator<<(const EMetadata& value)
{
   // Add the debug header.
   EDEBUG_FUNC(this,&value);

   // Append new metadata value to array.
   QList<EMetadata>::operator<<(value);

   // Return reference to this object.
   return *this;
}






/*!
 * Tests if the array is empty or not.
 *
 * @return Returns true of the array is empty else returns false.
 */
bool EMetaArray::isEmpty() const
{
   EDEBUG_FUNC(this);
   return QList<EMetadata>::isEmpty();
}






/*!
 * Gets the size of the array.
 *
 * @return Size of the array.
 */
int EMetaArray::size() const
{
   EDEBUG_FUNC(this);
   return QList<EMetadata>::size();
}






/*!
 * Returns iterator to first metadata item in the array.
 *
 * @return First metadata iterator.
 */
QList<EMetadata>::iterator EMetaArray::begin()
{
   EDEBUG_FUNC(this);
   return QList<EMetadata>::begin();
}






/*!
 * Returns constant iterator to first metadata object in the array.
 *
 * @return First metadata constant iterator.
 */
QList<EMetadata>::const_iterator EMetaArray::begin() const
{
   EDEBUG_FUNC(this);
   return QList<EMetadata>::begin();
}






/*!
 * Returns constant iterator to first metadata object in the array.
 *
 * @return First metadata constant iterator.
 */
QList<EMetadata>::const_iterator EMetaArray::cbegin() const
{
   EDEBUG_FUNC(this);
   return QList<EMetadata>::cbegin();
}






/*!
 * Returns end of array iterator.
 *
 * @return End of array iterator.
 */
QList<EMetadata>::iterator EMetaArray::end()
{
   EDEBUG_FUNC(this);
   return QList<EMetadata>::end();
}






/*!
 * Returns end of array constant iterator.
 *
 * @return End of array constant iterator.
 */
QList<EMetadata>::const_iterator EMetaArray::end() const
{
   EDEBUG_FUNC(this);
   return QList<EMetadata>::end();
}






/*!
 * Returns end of array constant iterator.
 *
 * @return End of array constant iterator.
 */
QList<EMetadata>::const_iterator EMetaArray::cend() const
{
   EDEBUG_FUNC(this);
   return QList<EMetadata>::cend();
}






/*!
 * Returns constant reference to metadata object with given index within the
 * array of metadata objects. If the index is beyond the scope of the array then
 * Qt will force exit the program.
 *
 * @param index Index of requested metadata object within the array of objects.
 *
 * @return Reference to metadata object with given index.
 */
const EMetadata& EMetaArray::at(int index) const
{
   EDEBUG_FUNC(this,index);
   return QList<EMetadata>::at(index);
}






/*!
 * Inserts a new metadata value to the array at the given index of the array. If
 * the index is less than or equal to 0 it is prepended to the array. If the
 * index is greater than or equal to the size of the array it is appended to the
 * list.
 *
 * @param index Index where new metadata value will be inserted.
 *
 * @param value The new metadata value to be inserted into the array.
 */
void EMetaArray::insert(int index, const EMetadata& value)
{
   EDEBUG_FUNC(this,index,&value);
   QList<EMetadata>::insert(index,value);
}






/*!
 * Appends a new metadata value to the end of the array.
 *
 * @param value The new metadata value to be appended to the end of the array.
 */
void EMetaArray::append(const EMetadata& value)
{
   EDEBUG_FUNC(this,&value);
   QList<EMetadata>::append(value);
}






/*!
 * Deletes all metadata values from the array making it an empty array.
 */
void EMetaArray::clear()
{
   EDEBUG_FUNC(this);
   QList<EMetadata>::clear();
}

