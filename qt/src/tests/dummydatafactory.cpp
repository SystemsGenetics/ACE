#include "dummydatafactory.h"
#include "dummydata.h"






std::unique_ptr<EAbstractData> DummyDataFactory::make(quint16 type) noexcept
{
   Q_UNUSED(type);
   return std::unique_ptr<EAbstractData>(new DummyData);
}
