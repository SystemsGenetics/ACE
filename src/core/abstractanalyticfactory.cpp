#include "abstractanalyticfactory.h"
#include "abstractanalytic.h"
#include "exception.h"



using namespace std;
unique_ptr<EAbstractAnalyticFactory> EAbstractAnalyticFactory::_instance {nullptr};






EAbstractAnalyticFactory& EAbstractAnalyticFactory::getInstance()
{
   if ( !_instance )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Analytic Factory Error"));
      e.setDetails(QObject::tr("Attempting to get abstract analytic factory when none has been set."));
      throw e;
   }
   return *_instance;
}






void EAbstractAnalyticFactory::setInstance(unique_ptr<EAbstractAnalyticFactory>&& factory)
{
   // Go through all possible analytics and make sure they all have unique command line names
   QMap<QString,bool> sanity;
   for (int i = 0; i < factory->getCount() ;++i)
   {
      QString name = factory->getCommandName(i);
      if ( sanity.contains(name) )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("Analytic Factory Error"));
         e.setDetails(QObject::tr("Detected two different analytics that conflict with the same"
                                  " command line name."));
         throw e;
      }
      sanity.insert(name,true);

      // go through all arguments of analytic and make sure they all have unique command line names
      QMap<QString,bool> sanity;
      unique_ptr<EAbstractAnalytic> test(factory->make(i));
      for (int i = 0; i < test->getArgumentCount() ;++i)
      {
         QString name = test->getArgumentData(i,EAbstractAnalytic::Role::CommandLineName)
               .toString();
         if ( sanity.contains(name) )
         {
            E_MAKE_EXCEPTION(e);
            e.setTitle(QObject::tr("Analytic Factory Error"));
            e.setDetails(QObject::tr("Detected two different arguments of an analytic that conflict"
                                     " with the same command line argument name."));
            throw e;
         }
         sanity.insert(name,true);
      }
   }

   // set new instance
   _instance = move(factory);
}
