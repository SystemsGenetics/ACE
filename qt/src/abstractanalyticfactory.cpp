#include "abstractanalyticfactory.h"



AbstractAnalyticFactory* AbstractAnalyticFactory::_instance = nullptr;






AbstractAnalyticFactory& AbstractAnalyticFactory::getInstance()
{
   return *_instance;
}






void AbstractAnalyticFactory::setInstance(std::unique_ptr<AbstractAnalyticFactory>&& factory)
{
   if ( _instance )
   {
      delete _instance;
   }
   _instance = factory.release();
}
