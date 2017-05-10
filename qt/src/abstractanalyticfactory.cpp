#include "abstractanalyticfactory.h"
#include "exception.h"



using namespace std;
unique_ptr<AbstractAnalyticFactory> AbstractAnalyticFactory::_instance {nullptr};






AbstractAnalyticFactory& AbstractAnalyticFactory::getInstance()
{
   if ( !_instance )
   {
      MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Analytic Factory Error"));
      e.out() << QObject::tr("Attempting to get abstract analytic factory when none has been set.");
      throw e;
   }
   return *_instance;
}






void AbstractAnalyticFactory::setInstance(unique_ptr<AbstractAnalyticFactory>&& factory)
{
   _instance = move(factory);
}
