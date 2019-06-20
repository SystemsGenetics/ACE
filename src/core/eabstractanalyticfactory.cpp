#include "eabstractanalyticfactory.h"
#include "eabstractanalytic.h"
#include "eabstractanalyticinput.h"
#include "eexception.h"
using namespace std;



/*!
 * Pointer to the global instance of this program's analytic factory.
 */
EAbstractAnalyticFactory* EAbstractAnalyticFactory::_instance {nullptr};






/*!
 * Returns a reference to the global instance of the class that implements this
 * factory. If no instance has ever been set than an exception is thrown.
 *
 * @return Reference to the global instance of this object's implementation.
 */
EAbstractAnalyticFactory& EAbstractAnalyticFactory::instance()
{
   // If the global instance pointer is null then throw an exception, else go to the
   // next step.
   if ( !_instance )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Logic Error"));
      e.setDetails(QObject::tr("Cannot get abstract analytic factory when none has been set."));
      throw e;
   }

   // Return reference to the global instance.
   return *_instance;
}






/*!
 * Sets the global instance for this object's implementation. If a global
 * instance already exists then it is deleted and replaced with the new one
 * given. This will also sanity check the analytic factory to make sure there
 * are no command line conflicts within the factory and its analytic
 * implementations. If a conflict is found then an exception is thrown.
 *
 * @param instance Pointer to the new global instance for this factory.
 */
void EAbstractAnalyticFactory::setInstance(std::unique_ptr<EAbstractAnalyticFactory>&& instance)
{
   // Check command line names for the given analytic factory, delete any existing
   // analytic factory, and set the global pointer to the new analytic factory given.
   checkCommandNames(instance.get());
   delete _instance;
   _instance = instance.release();
}






/*!
 * Confirms that all analytic command line names of the given analytic factory
 * are unique and then all argument command line names for each analytic is
 * unique. If a collision is detected then an exception is thrown.
 *
 * @param factory Pointer to the analytic factory whose analytic names are
 *                checked.
 */
void EAbstractAnalyticFactory::checkCommandNames(EAbstractAnalyticFactory* factory)
{
   // Iterate through all analytic objects the given factory that make for the
   // proceeding steps.
   QMap<QString,bool> sanity;
   for (quint16 i = 0; i < factory->size() ;++i)
   {
      // If the command line name of the analytic already exists for another analytic
      // then throw an exception, else go to the next step.
      QString name = factory->commandName(i);
      if ( sanity.contains(name) )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Logic Error"));
         e.setDetails(QObject::tr("Detected two different analytics that conflict with the same command line name."));
         throw e;
      }

      // Insert the command line name of the analytic to the mapping of names and check
      // that the analytic argument command line names are all unique for the analytic.
      sanity.insert(name,true);
      unique_ptr<EAbstractAnalytic> test(factory->make(i));
      checkCommandArguments(test.get());
   }
}






/*!
 * Confirms all argument command line names for the given analytic is unique. If
 * a collision is detected then an exception is thrown.
 *
 * @param analytic Pointer to the abstract analytic whose arguments are checked.
 */
void EAbstractAnalyticFactory::checkCommandArguments(EAbstractAnalytic* analytic)
{
   // Create an abstract analytic input from the given analytic and iterate through
   // all its arguments for the proceeding steps.
   EAbstractAnalyticInput* input {analytic->makeInput()};
   QMap<QString,bool> sanity;
   for (int i = 0; i < input->size() ;++i)
   {
      // If the command line name of the argument already exists for another argument
      // then throw an exception, else go the next step.
      QString name = input->data(i,EAbstractAnalyticInput::Role::CommandLineName).toString();
      if ( sanity.contains(name) )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Logic Error"));
         e.setDetails(QObject::tr("Detected two different arguments of an analytic that conflict with the same argument name."));
         throw e;
      }

      // Insert the command line name of the argument to the mapping of names.
      sanity.insert(name,true);
   }
}

