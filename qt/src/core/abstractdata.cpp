#include "abstractdata.h"
#include "dataobject.h"
#include "exception.h"






void EAbstractData::initialize(Ace::DataObject* object, EDataStream* stream)
{
   _data = object;
   _stream = stream;
}






EDataStream& EAbstractData::stream()
{
   if ( !_stream )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Abstract Data Interface Error"));
      e.setDetails(QObject::tr("Requesting reference to stream object of uninitialized abstract data"
                               " object."));
      throw e;
   }
   return *_stream;
}






bool EAbstractData::seek(quint64 offset)
{
   if ( !_data )
   {
      return false;
   }
   return _data->seek(offset);
}






bool EAbstractData::allocate(quint64 size)
{
   if ( !_data )
   {
      return false;
   }
   return _data->allocate(size);
}
