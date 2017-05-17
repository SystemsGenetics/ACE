#include "data.h"






void Data::readData()
{
   quint32 amount {0};
   stream() >> amount;
   while ( amount )
   {
      qint32 value;
      stream() >> value;
      _numbers.push_back(value);
      --amount;
   }
}






quint64 Data::getDataEnd() const
{
   return static_cast<quint64>(_numbers.size())*4+4;
}






void Data::newData()
{
   seek(0);
   quint32 amount {0};
   stream() << amount;
}






void Data::prepare(bool /*preAllocate*/)
{}






void Data::finish()
{
   quint32 amount {static_cast<quint32>(_numbers.size())};
   seek(0);
   stream() << amount;
   for (auto i = _numbers.begin(); i != _numbers.end() ;++i)
   {
      qint32 value {static_cast<qint32>(*i)};
      stream() << value;
   }
}
