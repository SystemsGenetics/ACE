#ifndef DATA_H
#define DATA_H
#include <QtCore>
#include <memory>

#include "utilities.h"



class DataStream
{
public:
   enum Status
   {
      Ok = 0
      ,ReadFailed
      ,CorruptData
      ,WriteFailed
      ,StringTooBig
   };
   DataStream(QFile* file);
   ~DataStream();
   MAKE_NO_COPY_OR_MOVE(DataStream);
   Status getStatus() const;
   operator bool() const;
   DataStream& operator<<(qint8 value);
   DataStream& operator<<(quint8 value);
   DataStream& operator<<(qint16 value);
   DataStream& operator<<(quint16 value);
   DataStream& operator<<(qint32 value);
   DataStream& operator<<(quint32 value);
   DataStream& operator<<(qint64 value);
   DataStream& operator<<(quint64 value);
   DataStream& operator<<(float value);
   DataStream& operator<<(double value);
   DataStream& operator<<(const QString& value);
   DataStream& operator<<(const QPixmap& value);
   DataStream& operator>>(qint8& value);
   DataStream& operator>>(quint8& value);
   DataStream& operator>>(qint16& value);
   DataStream& operator>>(quint16& value);
   DataStream& operator>>(qint32& value);
   DataStream& operator>>(quint32& value);
   DataStream& operator>>(qint64& value);
   DataStream& operator>>(quint64& value);
   DataStream& operator>>(float& value);
   DataStream& operator>>(double& value);
   DataStream& operator>>(QString& value);
   DataStream& operator>>(QPixmap& value);
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
   Status _status {Ok};
   QChar _stringBuffer[_stringBufferSize];
};



#endif
