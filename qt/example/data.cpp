#include "data.h"






void Data::readData()
{
   // Read size of integer array in file
   quint32 amount {0};
   stream() >> amount;

   // Check read was successful
   if ( !stream() )
   {
      // If failure occured create exception to report failure
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("IO Error"));
      e.out() << QObject::tr("Error reading from file.");
      throw e;
   }

   // Read in all integer values
   while ( amount )
   {
      // Read in value
      qint32 value;
      stream() >> value;

      // Check read was successful
      if ( !stream() )
      {
         // If failure occured create exception to report failure
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("IO Error"));
         e.out() << QObject::tr("Error reading from file.");
         throw e;
      }

      // Add read value to list.
      _numbers.push_back(value);
      --amount;
   }
}






quint64 Data::getDataEnd() const
{
   // Report offset at end of file data
   return static_cast<quint64>(_numbers.size())*4+4;
}






void Data::newData()
{
   // Seek to beginning of file, making sure it was successful
   if ( !seek(0) )
   {
      // If failure occured create exception to report failure
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("IO Error"));
      e.out() << QObject::tr("Error setting cursor position in file.");
      throw e;
   }

   // Set amount of integers stored to zero and write to file
   quint32 amount {0};
   stream() << amount;

   // Check write was successful
   if ( !stream() )
   {
      // If failure occured create exception to report failure
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
      // Seek to beginning of file, making sure it worked
      if ( !seek(0) )
      {
         // If failure occured create exception to report failure
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("IO Error"));
         e.out() << QObject::tr("Error setting cursor position in file.");
         throw e;
      }

      // Allocate all space required for new size after input, making sure it worked
      if ( !allocate(4+4*_futureSize) )
      {
         // If failure occured create exception to report failure
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("IO Error"));
         e.out() << QObject::tr("Error allocating new space in file.");
         throw e;
      }
   }
}






void Data::finish()
{
   // Seek to beginning of file, making sure it worked
   if ( !seek(0) )
   {
      // If failure occured create exception to report failure
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("IO Error"));
      e.out() << QObject::tr("Error setting cursor position in file.");
      throw e;
   }

   // Write new size of integer array to file
   quint32 amount {static_cast<quint32>(_numbers.size())};
   stream() << amount;

   // Make sure write worked
   if ( !stream() )
   {
      // If failure occured create exception to report failure
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("IO Error"));
      e.out() << QObject::tr("Error writing to file.");
      throw e;
   }

   // Iterate through all new values
   for (auto i = _numbers.begin(); i != _numbers.end() ;++i)
   {
      // Write new value to file
      qint32 value {static_cast<qint32>(*i)};
      stream() << value;

      // Make sure write worked
      if ( !stream() )
      {
         // If failure occured create exception to report failure
         E_MAKE_EXCEPTION(e);
         e.setTitle(QObject::tr("IO Error"));
         e.out() << QObject::tr("Error writing to file.");
         throw e;
      }
   }
}
