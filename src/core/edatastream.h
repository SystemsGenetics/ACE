#ifndef EDATASTREAM_H
#define EDATASTREAM_H
#include <QtCore>
#include <memory>
#include "edatastream_ext.h"
#include "ace_dataobject.h"
#include "ace.h"
#include "edebug.h"



/*!
 * This provides a C++ style stream for input and output of a data object. This
 * also provides endian compatibility for cross platform loading. All data
 * written by this data stream is in beg endian and all reading is switched back
 * to little endian if the local machine is little endian. If the local machine
 * is big endian then no switching of bits is done. Therefore means data object
 * files are always big endian, also called network endian. This class handles
 * all read operations from the data object as constant to keep in line with the
 * constant paradigm used in data objects and ACE in general.
 */
class EDataStream : public QObject
{
   Q_OBJECT
public:
   /*!
    * Defines special byte markers used to identify strings and byte arrays in this
    * data stream.
    */
   enum class Tag
   {
      /*!
       * Special unsigned byte value that marks the beginning of a string in the
       * stream.
       */
      String = 85
      /*!
       * Special unsigned byte value that marks the beginning of a byte array in the
       * stream.
       */
      ,ByteArray = 170
   };
public:
   const EDataStream& operator>>(qint8& value) const;
   const EDataStream& operator>>(qint16& value) const;
   const EDataStream& operator>>(qint32& value) const;
   const EDataStream& operator>>(qint64& value) const;
   const EDataStream& operator>>(quint8& value) const;
   const EDataStream& operator>>(quint16& value) const;
   const EDataStream& operator>>(quint32& value) const;
   const EDataStream& operator>>(quint64& value) const;
   const EDataStream& operator>>(float& value) const;
   const EDataStream& operator>>(double& value) const;
   const EDataStream& operator>>(QString& value) const;
   const EDataStream& operator>>(QByteArray& value) const;
   EDataStream& operator<<(qint8 value);
   EDataStream& operator<<(qint16 value);
   EDataStream& operator<<(qint32 value);
   EDataStream& operator<<(qint64 value);
   EDataStream& operator<<(quint8 value);
   EDataStream& operator<<(quint16 value);
   EDataStream& operator<<(quint32 value);
   EDataStream& operator<<(quint64 value);
   EDataStream& operator<<(float value);
   EDataStream& operator<<(double value);
   EDataStream& operator<<(const QString& value);
   EDataStream& operator<<(const QByteArray& value);
public:
   explicit EDataStream(Ace::DataObject* parent);
private:
   template<class T> const EDataStream& read(T& value) const;
   template<class T> EDataStream& write(T value);
   /*!
    * A pointer to this data stream's data object that is used for all input/output
    * streaming.
    */
   Ace::DataObject* _parent;
};






/*!
 * Reads in data from this data stream setting the given value to the data read
 * in.
 *
 * @tparam T Data type for the given value that is read in from this data
 *           stream.
 *
 * @param value Reference to the value that is read in from this data stream.
 *
 * @return Read only reference to this data stream.
 */
template<class T> const EDataStream& EDataStream::read(T& value) const
{
   EDEBUG_FUNC(this,&value)

   // Read in the given value from this data stream, flipping the bytes of the read
   // in value if required.
   _parent->read(reinterpret_cast<char*>(&value),sizeof(T));
   value = qFromBigEndian(value);

   // Return a reference to this data stream.
   return *this;
}






/*!
 * Writes out the given value to this data stream's data object.
 *
 * @tparam T The type for the given value that is written.
 *
 * @param value The value that is written to this data stream's data object.
 *
 * @return Reference to this data stream.
 */
template<class T> EDataStream& EDataStream::write(T value)
{
   EDEBUG_FUNC(this,&value)

   // Write out the given value to the data stream, flipping the bytes before writing
   // if required.
   value = qToBigEndian(value);
   _parent->write(reinterpret_cast<char*>(&value),sizeof(T));

   // Return a reference to this data stream.
   return *this;
}

#endif
