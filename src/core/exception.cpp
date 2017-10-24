#include "exception.h"






EException ESilent::getException() const noexcept
{
   if ( !_exception )
   {
      return EException();
   }
   return *_exception;
}






void ESilent::setException(EException exception) const noexcept
{
   _exception.reset(new EException(exception));
   _hasException = true;
}
