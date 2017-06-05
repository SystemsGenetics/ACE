#include "abstractanalyticfactory.h"
#include "exception.h"



using namespace std;
unique_ptr<EAbstractAnalyticFactory> EAbstractAnalyticFactory::_instance {nullptr};






EAbstractAnalyticFactory& EAbstractAnalyticFactory::getInstance()
{
   if ( !_instance )
   {
      E_MAKE_EXCEPTION(e);
      e.setLevel(EException::Critical);
      e.setType(NoFactorySet);
      e.setTitle(QObject::tr("Analytic Factory Error"));
      e.setDetails(QObject::tr("Attempting to get abstract analytic factory when none has been set."));
      throw e;
   }
   return *_instance;
}






void EAbstractAnalyticFactory::setInstance(unique_ptr<EAbstractAnalyticFactory>&& factory)
{
   _instance = move(factory);
}
