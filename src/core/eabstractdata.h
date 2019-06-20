#ifndef EABSTRACTDATA_H
#define EABSTRACTDATA_H
#include <QtCore>
#include <memory>
#include "emetadata.h"
#include "eexception.h"
#include "global.h"



/*!
 * This represents a data object for the ACE system. Implementations of this
 * class are responsible for managing a data type for a program using the ACE
 * library. Implementations can also manipulate the user metadata of its data
 * object. All functionality is provided through protected methods. If an
 * already existing data object is opened only the read data interface is
 * called. If it is a new data object then the write new data and finish
 * interfaces are called in that order. Data object files are organized into
 * three parts; the header, data, and user metadata in that order. An
 * implementation of this class is responsible for the data section.
 */
class EAbstractData : public QObject
{
   Q_OBJECT
public:
   /*!
    * This interface returns the index within this data object after the end of
    * where the data is stored. This is used to determine where the user metadata
    * is written since it comes after the data section of the data object.
    *
    * @return Index of the first byte in the data object after the data section
    *         ends.
    */
   virtual qint64 dataEnd() const = 0;
   /*!
    * This interface reads in the data of an existing data object that was just
    * opened.
    */
   virtual void readData() = 0;
   /*!
    * This interface is called to initialize the data object's data to a null
    * state.
    */
   virtual void writeNewData() = 0;
   virtual void finish();
   /*!
    * This interface returns a qt table model that represents the data of this data
    * object as a table.
    *
    * @return Pointer to model that represents the data of this data object.
    */
   virtual QAbstractTableModel* model() = 0;
public:
   template<class T> const T* cast() const;
   template<class T> T* cast();
protected:
   const EMetadata& systemMeta() const;
   const EMetadata& meta() const;
   void setMeta(const EMetadata& newMeta);
   const EDataStream& stream() const;
   EDataStream& stream();
   void seek(qint64 index) const;
   void allocate(qint64 size);
};






/*!
 * Casts this abstract data object into another constant class type.
 *
 * @tparam T The type this abstract block is being cast to. This should be this
 *           block's implementation type.
 *
 * @return Cast constant pointer to this object.
 */
template<class T> const T* EAbstractData::cast() const
{
   // Use qt object cast to cast this object to the given template type as read only,
   // making sure it worked.
   const T* ret {qobject_cast<const T*>(this)};
   if ( !ret )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Cannot convert abstract data object to given type."));
      throw e;
   }

   // Return the cast pointer.
   return ret;
}






/*!
 * Casts this abstract data object into another class type.
 *
 * @tparam T The type this abstract block is being cast to. This should be this
 *           block's implementation type.
 *
 * @return Cast pointer to this object.
 */
template<class T> T* EAbstractData::cast()
{
   // Use qt object cast to cast this object to the given template type, making sure
   // it worked.
   T* ret {qobject_cast<T*>(this)};
   if ( !ret )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Cannot convert abstract data object to given type."));
      throw e;
   }

   // Return the cast pointer.
   return ret;
}

#endif
