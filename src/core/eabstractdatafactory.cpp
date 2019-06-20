#include "eabstractdatafactory.h"
#include "eexception.h"



/*!
 * Pointer to the global instance of this program's data factory.
 */
EAbstractDataFactory* EAbstractDataFactory::_instance {nullptr};






/*!
 * Returns a reference to the global instance of the class that implements this
 * factory. If no instance has ever been set than an exception is thrown.
 *
 * @return Reference to the global instance of this object's implementation.
 */
EAbstractDataFactory& EAbstractDataFactory::instance()
{
   // If the global instance pointer is null then throw an exception, else go the
   // next step.
   if ( !_instance )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Analytic Factory Error"));
      e.setDetails(QObject::tr("Attempting to get abstract analytic factory when none has been set."));
      throw e;
   }

   // Return reference to the global instance.
   return *_instance;
}






/*!
 * Sets the global instance for this object's implementation. If a global
 * instance already exists then it is deleted and replaced with the new one
 * given.
 *
 * @param instance Pointer to the new global instance for this factory.
 */
void EAbstractDataFactory::setInstance(std::unique_ptr<EAbstractDataFactory>&& instance)
{
   // Delete any existing global instance and set the global pointer to the new given
   // instance.
   delete _instance;
   _instance = instance.release();
}

