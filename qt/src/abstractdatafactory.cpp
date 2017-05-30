#include "abstractdatafactory.h"
#include "exception.h"



using namespace std;
unique_ptr<EAbstractDataFactory> EAbstractDataFactory::_instance {nullptr};






EAbstractDataFactory& EAbstractDataFactory::getInstance()
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






void EAbstractDataFactory::setInstance(unique_ptr<EAbstractDataFactory>&& factory)
{
   _instance = move(factory);
}
