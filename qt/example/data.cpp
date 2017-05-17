#include "data.h"






void Data::readData()
{
   quint32 amount {0};
   stream() >> amount;
   if ( !stream() )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("IO Error"));
      e.out() << QObject::tr("Error reading from file.");
      throw e;
   }
   while ( amount )
   {
      qint32 value;
      stream() >> value;
      if ( !stream() )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("IO Error"));
         e.out() << QObject::tr("Error reading from file.");
         throw e;
      }
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
   if ( !seek(0) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("IO Error"));
      e.out() << QObject::tr("Error setting cursor position in file.");
      throw e;
   }
   quint32 amount {0};
   stream() << amount;
   if ( !stream() )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("IO Error"));
      e.out() << QObject::tr("Error writing to file.");
      throw e;
   }
}






void Data::prepare(bool preAllocate)
{
   if ( preAllocate )
   {
      if ( !seek(0) )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("IO Error"));
         e.out() << QObject::tr("Error setting cursor position in file.");
         throw e;
      }
      if ( !allocate(4+4*_futureSize) )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("IO Error"));
         e.out() << QObject::tr("Error allocating new space in file.");
         throw e;
      }
   }
}






void Data::finish()
{
   quint32 amount {static_cast<quint32>(_numbers.size())};
   if ( !seek(0) )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("IO Error"));
      e.out() << QObject::tr("Error setting cursor position in file.");
      throw e;
   }
   stream() << amount;
   if ( !stream() )
   {
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("IO Error"));
      e.out() << QObject::tr("Error writing to file.");
      throw e;
   }
   for (auto i = _numbers.begin(); i != _numbers.end() ;++i)
   {
      qint32 value {static_cast<qint32>(*i)};
      stream() << value;
      if ( !stream() )
      {
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("IO Error"));
         e.out() << QObject::tr("Error writing to file.");
         throw e;
      }
   }
}
