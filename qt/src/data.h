#ifndef DATA_H
#define DATA_H
#include <QtCore>
#include <memory>



class Data
{
public:
   enum class Status
   {
      Ok = 0
      ,AllocateFailed
      ,ClearFailed
      ,ReadFailed
      ,CorruptData
      ,WriteFailed
      ,StringTooBig
   };
   ~Data();
   Data(const Data&) = delete;
   Data& operator=(const Data&) = delete;
   Data(Data&&) = delete;
   Data& operator=(Data&&) = delete;
   static std::unique_ptr<Data> open(const QString& path);
   quint64 allocate(quint64 size);
   quint64 getUsedSize() const;
   quint64 getFreeSize() const;
   quint64 getTotalSize() const;
   Status getStatus() const;
   bool seek(quint64 location);
   void clear();
   Data& operator<<(qint8 value);
   Data& operator<<(quint8 value);
   Data& operator<<(qint16 value);
   Data& operator<<(quint16 value);
   Data& operator<<(qint32 value);
   Data& operator<<(quint32 value);
   Data& operator<<(qint64 value);
   Data& operator<<(quint64 value);
   Data& operator<<(float value);
   Data& operator<<(double value);
   Data& operator<<(const QString& value);
   Data& operator<<(const QPixmap& value);
   Data& operator>>(qint8& value);
   Data& operator>>(quint8& value);
   Data& operator>>(qint16& value);
   Data& operator>>(quint16& value);
   Data& operator>>(qint32& value);
   Data& operator>>(quint32& value);
   Data& operator>>(qint64& value);
   Data& operator>>(quint64& value);
   Data& operator>>(float& value);
   Data& operator>>(double& value);
   Data& operator>>(QString& value);
   Data& operator>>(QPixmap& value);
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
   Data(QFile* file, quint64 usedSize);
   QFile* _file;
   quint64 _usedSize;
   Status _status {Status::Ok};
   QChar _stringBuffer[_stringBufferSize];
};



#endif
