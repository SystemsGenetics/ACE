#ifndef DATA_H
#define DATA_H
#include <QtCore>
#include <memory>



class Data
{
public:
   enum class Type
   {
      Used
      ,Free
      ,Total
   };
   enum class Status
   {
      Ok
      ,AllocateFailed
      ,SeekFailed
      ,ReadPastEnd
      ,WrotePastEnd
      ,ReadFailed
      ,WriteFailed
   };
   ~Data();
   Data(const Data&) = delete;
   Data& operator=(const Data&) = delete;
   Data(Data&&) = delete;
   Data& operator=(Data&&) = delete;
   static std::unique_ptr<Data> open(const QString& path);
   quint64 allocate(quint64 size);
   quint64 getSize(MemoryType type) const;
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
   Data& operator>>(qint8 value);
   Data& operator>>(quint8 value);
   Data& operator>>(qint16 value);
   Data& operator>>(quint16 value);
   Data& operator>>(qint32 value);
   Data& operator>>(quint32 value);
   Data& operator>>(qint64 value);
   Data& operator>>(quint64 value);
   Data& operator>>(float value);
   Data& operator>>(double value);
   Data& operator>>(const QString& value);
private:
   Data(QFile* file, quint64 usedSize);
   QFile* _file;
   quint64 _usedSize;
};



#endif
