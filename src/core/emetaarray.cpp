#include "emetaarray.h"






/*!
 * Tests if the array is empty or not. 
 *
 * @return Returns true of the array is empty else returns false. 
 */
bool EMetaArray::isEmpty() const
{
   return QList<EMetadata>::isEmpty();
}






/*!
 * Gets the size of the array. 
 *
 * @return Size of the array. 
 */
int EMetaArray::size() const
{
   return QList<EMetadata>::size();
}






/*!
 * Returns iterator to first metadata item in the array. 
 *
 * @return First metadata iterator. 
 */
QList<EMetadata>::iterator EMetaArray::begin()
{
   return QList<EMetadata>::begin();
}






/*!
 * Returns constant iterator to first metadata object in the array. 
 *
 * @return First metadata constant iterator. 
 */
QList<EMetadata>::const_iterator EMetaArray::begin() const
{
   return QList<EMetadata>::begin();
}






/*!
 * Returns constant iterator to first metadata object in the array. 
 *
 * @return First metadata constant iterator. 
 */
QList<EMetadata>::const_iterator EMetaArray::cbegin() const
{
   return QList<EMetadata>::cbegin();
}






/*!
 * Returns end of array iterator. 
 *
 * @return End of array. 
 */
QList<EMetadata>::iterator EMetaArray::end()
{
   return QList<EMetadata>::end();
}






/*!
 * Returns end of array constant iterator. 
 *
 * @return End of array. 
 */
QList<EMetadata>::const_iterator EMetaArray::end() const
{
   return QList<EMetadata>::end();
}






/*!
 * Returns end of array constant iterator. 
 *
 * @return End of array. 
 */
QList<EMetadata>::const_iterator EMetaArray::cend() const
{
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
   QList<EMetadata>::insert(index,value);
}






/*!
 * Appends a new metadata value to the end of the array. 
 *
 * @param value The new metadata value to be appended to the end of the array. 
 */
void EMetaArray::append(const EMetadata& value)
{
   QList<EMetadata>::append(value);
}






/*!
 * Deletes all metadata values from the array making it an empty array. 
 */
void EMetaArray::clear()
{
   QList<EMetadata>::clear();
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
   return QList<EMetadata>::operator[](index);
}






/*!
 * Appends a new metadata value to the end of the array. 
 *
 * @param value The new metadata value to be appended to the end of the array. 
 *
 * @return Reference to this object. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Append new metadata value to array. 
 *
 * 2. Return reference to this object. 
 */
EMetaArray& EMetaArray::operator<<(const EMetadata& value)
{
   QList<EMetadata>::operator<<(value);
   return *this;
}
