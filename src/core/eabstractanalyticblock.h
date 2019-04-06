#ifndef EABSTRACTANALYTICBLOCK_H
#define EABSTRACTANALYTICBLOCK_H
#include <QObject>
#include "eexception.h"
#include "global.h"



/*!
 * This represents a block of work or results that an analytic produces and
 * consumes. This class handles storing the data needed for work and result
 * blocks for an analytic. This also handles the conversion to and from a byte
 * array so blocks can be communicated between different processes if MPI or
 * chunk running is used.
 */
class EAbstractAnalyticBlock : public QObject
{
   Q_OBJECT
public:
   static int extractIndex(const QByteArray& data);
public:
   /*!
    * Constructs an uninitialized block. This is used in MPI and chunk runs to load
    * blocks from byte arrays.
    */
   explicit EAbstractAnalyticBlock() = default;
   explicit EAbstractAnalyticBlock(int index);
   int index() const;
   template<class T> const T* cast() const;
   template<class T> T* cast();
   QByteArray toBytes() const;
   void fromBytes(const QByteArray& data);
protected:
   virtual void write(QDataStream& stream) const;
   virtual void read(QDataStream& stream);
private:
   friend EDebug& operator<<(EDebug&, const EAbstractAnalyticBlock*const);
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
 */
template<class T> const T* EAbstractAnalyticBlock::cast() const
{
   // Use qt object cast to cast this object to the given template type as read only,
   // making sure it worked.
   const T* ret {qobject_cast<const T*>(this)};
   if ( !ret )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Cannot convert abstract analytic block to given type."));
      throw e;
   }

   // Return the cast pointer.
   return ret;
}






/*!
 * Casts this abstract block object into another class type.
 *
 * @tparam T The type this abstract block is being cast to. This should be this
 *           block's implementation type.
 *
 * @return Cast pointer to this object.
 */
template<class T> T* EAbstractAnalyticBlock::cast()
{
   // Use qt object cast to cast this object to the given template type, making sure
   // it worked.
   T* ret {qobject_cast<T*>(this)};
   if ( !ret )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(tr("Logic Error"));
      e.setDetails(tr("Cannot convert abstract analytic block to given type."));
      throw e;
   }

   // Return the cast pointer.
   return ret;
}

#endif
