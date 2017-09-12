#ifndef EDATASTREAM_H
#define EDATASTREAM_H
#include <QtCore>
#include <memory>

#include "utilities.h"
#include "exception.h"



template <> inline float qbswap<float>(float source) { return source; }
template <> inline double qbswap<double>(double source) { return source; }



/// Data stream used for input/output of data objects. All output stream operators will do nothing
/// if the stream is in an error state and input stream operators will set their values to null.
class EDataStream : public ESilent
{
public:
   EDataStream(QFile* file): _file(file) {}
   ACE_DISBALE_COPY_AND_MOVE(EDataStream)
   template<class T> void write(const T* data, int size) { rawWrite(data,size*sizeof(T)); }
   template<class T> void read(T* data, int size) const { rawRead(data,size*sizeof(T)); }
   EDataStream& operator<<(qint8 value) { return writeValue(value); }
   EDataStream& operator<<(quint8 value) { return writeValue(value); }
   EDataStream& operator<<(qint16 value) { return writeValue(value,true); }
   EDataStream& operator<<(quint16 value) { return writeValue(value,true); }
   EDataStream& operator<<(qint32 value) { return writeValue(value,true); }
   EDataStream& operator<<(quint32 value) { return writeValue(value,true); }
   EDataStream& operator<<(qint64 value) { return writeValue(value,true); }
   EDataStream& operator<<(quint64 value) { return writeValue(value,true); }
   EDataStream& operator<<(float value) { return writeValue(value); }
   EDataStream& operator<<(double value) { return writeValue(value); }
   EDataStream& operator<<(const QString& value);
   EDataStream& operator<<(const QByteArray& value);
   const EDataStream& operator>>(qint8& value) const { return readValue(value); }
   const EDataStream& operator>>(quint8& value) const { return readValue(value); }
   const EDataStream& operator>>(qint16& value) const { return readValue(value,true); }
   const EDataStream& operator>>(quint16& value) const { return readValue(value,true); }
   const EDataStream& operator>>(qint32& value) const { return readValue(value,true); }
   const EDataStream& operator>>(quint32& value) const { return readValue(value,true); }
   const EDataStream& operator>>(qint64& value) const { return readValue(value,true); }
   const EDataStream& operator>>(quint64& value) const { return readValue(value,true); }
   const EDataStream& operator>>(float& value) const { return readValue(value); }
   const EDataStream& operator>>(double& value) const { return readValue(value); }
   const EDataStream& operator>>(QString& value) const;
   const EDataStream& operator>>(QByteArray& value) const;
private:
   enum
   {
      String = 85
      ,ByteArray = 170
   };
   template<class T> EDataStream& writeValue(T value, bool endian = false);
   template<class T> const EDataStream& readValue(T& value, bool endian = false) const;
   bool rawWrite(const void* data, int size);
   bool rawRead(void* data, int size) const;
   QFile* _file;
};






template<class T>
inline EDataStream& EDataStream::writeValue(T value, bool endian)
{
   // make sure stream is in ok state
   if ( *this )
   {
      // if value needs endian switching do so
      if ( endian )
      {
         value = qToBigEndian(value);
      }

      // write value to file
      rawWrite(&value,sizeof(T));
   }

   // return reference
   return *this;
}






template<class T>
inline const EDataStream& EDataStream::readValue(T& value, bool endian) const
{
   // make sure stream is in ok state
   if ( *this )
   {
      // read value and make sure it was successful
      if ( !rawRead(&value,sizeof(T)) )
      {
         value = 0;
         return *this;
      }

      // if value needs endian switching do so
      if ( endian )
      {
         value = qToBigEndian(value);
      }
   }

   // return reference
   return *this;
}



#endif
