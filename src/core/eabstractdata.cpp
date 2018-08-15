#include "eabstractdata.h"
#include "ace_dataobject.h"



//






/*!
 * This interface finalizes this data object's data after the analytic that created 
 * it has finished giving it new data. The default implementation does nothing. 
 */
void EAbstractData::finish()
{}






/*!
 * Returns the root metadata object for this data object's system metadata. 
 *
 * @return Root metadata object for this data object's system metadata. 
 */
EMetadata EAbstractData::systemMeta() const
{
   return qobject_cast<Ace::DataObject*>(parent())->systemMeta();
}






/*!
 * Returns the root metadata object for this data object's user metadata. 
 *
 * @return Root metadata object for this data object's user metadata. 
 */
EMetadata EAbstractData::meta() const
{
   return qobject_cast<Ace::DataObject*>(parent())->userMeta();
}






/*!
 * Sets this data object's user metadata to the given metadata as its new root. The 
 * given metadata must be an object type or else an exception is thrown. 
 *
 * @param newMeta New root metadata object for this data object's user metadata. 
 */
void EAbstractData::setMeta(const EMetadata& newMeta)
{
   qobject_cast<Ace::DataObject*>(parent())->setUserMeta(newMeta);
}






/*!
 * Returns a read only reference to the data stream of this data object. 
 *
 * @return Read only reference to this data object's data stream. 
 */
const EDataStream& EAbstractData::stream() const
{
   return qobject_cast<Ace::DataObject*>(parent())->stream();
}






/*!
 * Returns a reference to the data stream of this data object. 
 *
 * @return Reference to this data object's data stream. 
 */
EDataStream& EAbstractData::stream()
{
   return qobject_cast<Ace::DataObject*>(parent())->stream();
}






/*!
 * Changes the cursor position of this data object to the given index. The cursor 
 * position does not include the header of this data object which is hidden so 
 * index 0 is one byte after the header section. 
 *
 * @param index The index this data object's cursor position is set to. 
 */
void EAbstractData::seek(qint64 index) const
{
   qobject_cast<Ace::DataObject*>(parent())->seek(index);
}






/*!
 * Allocates the given number of bytes after this data object's cursor position to 
 * the underlying file. This means if the given size to allocate is less then the 
 * current number of bytes after this data object's cursor position then the 
 * underlying file will be truncated to the smaller size. 
 *
 * @param size The number of bytes to allocate after this object's current cursor 
 *             position. 
 */
void EAbstractData::allocate(qint64 size)
{
   qobject_cast<Ace::DataObject*>(parent())->allocate(size);
}
