#include <QtEndian>
#include <QPixmap>

#include "datastream.h"






EDataStream& EDataStream::operator<<(const QString& value)
{
   // make sure stream is in ok state
   if ( *this )
   {
      // write out string identifier type
      quint8 type = String;
      *this << type;
      if ( *this )
      {
         // write out string in UTF8 format
         *this << value.toUtf8();
      }
   }

   // return reference to stream
   return *this;
}






EDataStream& EDataStream::operator<<(const QByteArray& value)
{
   // make sure stream is in ok state
   if ( *this )
   {
      // write out byte array identifier type
      quint8 type = ByteArray;
      *this << type;
      if ( *this )
      {
         // write out size of byte array
         quint32 size = value.size();
         *this << size;
         if ( *this )
         {
            // write out byte array
            rawWrite(value.data(),value.size());
         }
      }
   }

   // return reference to stream
   return *this;
}






const EDataStream& EDataStream::operator>>(QString& value) const
{
   // clear input string
   value.clear();

   // make sure stream is in ok state
   if ( *this )
   {
      // read type identifier
      quint8 type;
      *this >> type;
      if ( *this )
      {
         // make sure it is correct type
         if ( type != String )
         {
            E_MAKE_EXCEPTION(e);
            e.setTitle(QObject::tr("Data Stream Read"));
            e.setDetails(QObject::tr("Could not read string because data is corrupt"));
            setException(e);
         }
         else
         {
            // read in string in UTF8 format
            QByteArray data;
            *this >> data;
            value = QString::fromUtf8(data);
         }
      }
   }

   // return reference to stream
   return *this;
}






const EDataStream& EDataStream::operator>>(QByteArray& value) const
{
   // clear input
   value.clear();

   // make sure stream is in ok state
   if ( *this )
   {
      // read type identifier
      quint8 type;
      *this >> type;
      if ( *this )
      {
         // make sure it is correct type
         if ( type != ByteArray )
         {
            E_MAKE_EXCEPTION(e);
            e.setTitle(QObject::tr("Data Stream Read"));
            e.setDetails(QObject::tr("Could not read byte array because data is corrupt."));
            setException(e);
         }
         else
         {
            // read size of byte array
            quint32 size;
            *this >> size;
            if ( *this )
            {
               // read byte array and make sure it worked
               value.resize(size);
               if ( !rawRead(value.data(),size) )
               {
                  E_MAKE_EXCEPTION(e);
                  e.setTitle(QObject::tr("Data Stream Read"));
                  e.setDetails(QObject::tr("Could not read byte array because data is corrupt."));
                  setException(e);
               }
            }
         }
      }
   }

   // return reference to stream
   return *this;
}






bool EDataStream::rawWrite(const void *data, int size)
{
   // write value to file checking if it worked
   if ( _file->write(reinterpret_cast<const char*>(data),size) != size )
   {
      // set stream to error state and return failure
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Data Stream Write"));
      e.setDetails(QObject::tr("Failed writing to file: %1").arg(_file->errorString()));
      setException(e);
      return false;
   }

   // return success
   return true;
}






bool EDataStream::rawRead(void *data, int size) const
{
   // read value from file checking if it worked
   if ( _file->read(reinterpret_cast<char*>(data),size) != size )
   {
      // set stream to error state and return failure
      E_MAKE_EXCEPTION(e);
      e.setTitle(QObject::tr("Data Stream Read"));
      e.setDetails(QObject::tr("Failed reading from file: %1").arg(_file->errorString()));
      setException(e);
      return false;
   }

   // return success
   return true;
}
