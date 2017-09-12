#include <QtEndian>
#include <QPixmap>

#include "datastream.h"



using namespace std;






EDataStream::EDataStream(QFile *file):
   _file(file)
{}

void EDataStream::ohmy(const char *data, int size)
{
   _file->write(data,size);
   _file->flush();
}






EDataStream& EDataStream::operator<<(const QString& value)
{
   // make sure stream is in ok state
   if ( *this )
   {
      // write out string identifier type
      quint8 type = String;
      if ( write(&type) )
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
      if ( write(&type) )
      {
         // write out size of byte array
         quint32 size = qToBigEndian((quint32)value.size());
         if ( write(&size) )
         {
            // write out byte array
            write(value.data(),value.size());
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
      quint8 type {0};
      if ( read(&type) )
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
      quint8 type {0};
      if ( read(&type) )
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
            if ( read(&size) )
            {
               size = qFromBigEndian(size);

               // read byte array and make sure it worked
               value.resize(size);
               if ( !read(value.data(),size) )
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
