#include "abstractdata.h"
#include "dataobject.h"
#include "exception.h"






void EAbstractData::initialize(Ace::DataObject* object, EDataStream* stream) noexcept
{
   // initialize data and stream pointers
   _data = object;
   _stream = stream;
   _meta = &(object->getMeta());
}






EDataStream& EAbstractData::stream()
{
   if ( !_stream )
   {
      E_MAKE_EXCEPTION(e);
      e.setLevel(EException::Critical);
      e.setType(NullReference);
      e.setTitle(QObject::tr("Abstract Data Interface"));
      e.setDetails(QObject::tr("Requesting reference to stream object of uninitialized abstract data"
                               " object."));
      throw e;
   }
   return *_stream;
}






bool EAbstractData::seek(quint64 offset) noexcept
{
   // make sure data reference exists
   if ( !_data )
   {
      return false;
   }

   // call seek of data object
   return _data->seek(offset);
}






bool EAbstractData::allocate(quint64 size) noexcept
{
   // make sure data reference exists
   if ( !_data )
   {
      return false;
   }

   // call allocate of data object
   return _data->allocate(size);
}
