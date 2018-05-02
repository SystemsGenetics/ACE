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
   static int extractIndex(const QByteArray& data);
   /*!
    * Constructs an uninitialized block. This is used in MPI and chunk runs to load 
    * blocks from byte arrays. 
    */
   explicit Block() = default;
   explicit Block(int index);
   int index() const;
   QByteArray toBytes() const;
   template<class T> const T* cast() const;
   void fromBytes(const QByteArray& data);
   template<class T> T* cast();
protected:
   virtual void write(QDataStream& stream) const;
   virtual void read(QDataStream& stream);
private:
   /*!
    * The index of this block. 
    */
   int _index {-1};
};






/*!
 * Casts this abstract block object into another constant class type. 
 *
 * @tparam T The type this abstract block is being cast to. This should be this 
 *           block's implementation type. 
 *
 * @return Cast constant pointer to this object. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Use qt object cast to cast this object to the given template type as read 
 *    only. If the cast fails then throw an exception, else return the cast read 
 *    only pointer. 
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
 * @tparam T The type this abstract block is being cast to. This should be this 
 *           block's implementation type. 
 *
 * @return Cast pointer to this object. 
 *
 *
 * Steps of Operation: 
 *
 * 1. Use qt object cast to cast this object to the given template type. If the 
 *    cast fails then throw an exception, else return the cast pointer. 
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
