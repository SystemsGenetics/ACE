#include "abstractdatafactory.h"
#include "exception.h"



using namespace std;
unique_ptr<AbstractDataFactory> AbstractDataFactory::_instance {nullptr};






AbstractDataFactory& AbstractDataFactory::getInstance()
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






void AbstractDataFactory::setInstance(unique_ptr<AbstractDataFactory>&& factory)
{
   _instance = move(factory);
}
