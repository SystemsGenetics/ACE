#ifndef EABSTRACTANALYTIC_BLOCK_H
#define EABSTRACTANALYTIC_BLOCK_H
#include "eabstractanalytic.h"
#include "eexception.h"
//



/*!
 * This represents a block of work or results that an analytic produces and 
 * consumes. This class handles storing the data needed for work and result blocks 
 * for an analytic. This also handles the conversion to and from a byte array so 
 * blocks can be communicated between different processes if MPI or chunk running 
 * is used. 
 */
class EAbstractAnalytic::Block : public QObject
{
   Q_OBJECT
public:
   /*!
    * This interface converts this block's data into a qt byte array. 
    *
    * @return Qt byte array encoded with this block's data. 
    */
   virtual QByteArray toBytes() const = 0;
   /*!
    * This interface loads data from the given qt byte array into this block, 
    * overwriting any data this block may possess. 
    *
    * @param data Qt byte array whose embedded data is loaded into this block. 
    */
   virtual void fromBytes(const QByteArray& data) = 0;
   template<class T> const T* cast() const;
   template<class T> T* cast();
};






/*!
 * Casts this abstract block object into another constant class type. 
 *
 * @tparam T  
 *
 * @return Cast constant pointer to this object. 
 */
template<class T> const T* EAbstractAnalytic::Block::cast() const
{
   const T* ret {qobject_cast<const T*>(this)};
   if ( !ret )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Cannot convert abstract analytic block to given type."));
      throw e;
   }
   return ret;
}






/*!
 * Casts this abstract block object into another class type. 
 *
 * @tparam T  
 *
 * @return Cast pointer to this object. 
 */
template<class T> T* EAbstractAnalytic::Block::cast()
{
   T* ret {qobject_cast<T*>(this)};
   if ( !ret )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Cannot convert abstract analytic block to given type."));
      throw e;
   }
   return ret;
}



#endif
