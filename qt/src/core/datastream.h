#ifndef EDATASTREAM_H
#define EDATASTREAM_H
#include <QtCore>
#include <memory>

#include "utilities.h"
#include "exception.h"



/// Data stream used for input/output of data objects. All output stream operators will do nothing
/// if the stream is in an error state and input stream operators will set their values to null.
class EDataStream : public ESilent
{
public:
   enum Errors
   {
      Ok = 0
      ,ReadFailed
      ,CorruptData
      ,WriteFailed
      ,StringTooBig
   };
   EDataStream(QFile* file);
   ACE_DISBALE_COPY_AND_MOVE(EDataStream)
   EDataStream& operator<<(qint8 value);
   EDataStream& operator<<(quint8 value);
   EDataStream& operator<<(qint16 value);
   EDataStream& operator<<(quint16 value);
   EDataStream& operator<<(qint32 value);
   EDataStream& operator<<(quint32 value);
   EDataStream& operator<<(qint64 value);
   EDataStream& operator<<(quint64 value);
   EDataStream& operator<<(float value);
   EDataStream& operator<<(double value);
   EDataStream& operator<<(const QString& value);
   EDataStream& operator<<(const QPixmap& value);
   EDataStream& operator>>(qint8& value);
   EDataStream& operator>>(quint8& value);
   EDataStream& operator>>(qint16& value);
   EDataStream& operator>>(quint16& value);
   EDataStream& operator>>(qint32& value);
   EDataStream& operator>>(quint32& value);
   EDataStream& operator>>(qint64& value);
   EDataStream& operator>>(quint64& value);
   EDataStream& operator>>(float& value);
   EDataStream& operator>>(double& value);
   EDataStream& operator>>(QString& value);
   EDataStream& operator>>(QPixmap& value);
private:
   enum
   {
      String = 85
      ,Pixmap = 170
   };
   constexpr static int _maxStringSize {65536};
   constexpr static int _stringBufferSize {1024};
   template<class T> bool write(T value, quint64 size = 1);
   template<class T> bool read(T* value, quint64 size = 1);
   QFile* _file;
   QChar _stringBuffer[_stringBufferSize];
};



#endif
