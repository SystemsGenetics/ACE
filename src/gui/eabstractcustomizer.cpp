#include "eabstractcustomizer.h"
#include "../core/eexception.h"
#include "ace_defaultcustomizer.h"



EAbstractCustomizer* EAbstractCustomizer::_instance {nullptr};



EAbstractCustomizer &EAbstractCustomizer::instance()
{
    if ( !_instance )
    {
        _instance = new Ace::DefaultCustomizer;
    }
    return *_instance;
}



void EAbstractCustomizer::setInstance(EAbstractCustomizer *instance)
{
    delete _instance;
    _instance = instance;
}
