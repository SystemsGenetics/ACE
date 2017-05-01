#ifndef DATA_H
#define DATA_H
#include <QtCore>



class Data
{
public:
   enum MemoryType
   {
      Used
      ,Free
      ,Total
   };
   Data() = default;
   ~Data();
   Data(const Data&) = delete;
   Data& operator=(const Data&) = delete;
   Data(Data&&) = delete;
   Data& operator=(Data&&) = delete;
   bool open(const QString& path, QIODevice::OpenModeFlag flags);
   quint64 allocate(quint64 size);
   quint64 size(MemoryType type) const;
   bool isGood() const;
   bool seek(quint64 location) const;
   QDataStream& stream();
   void clear();
private:
   QFile* _file {nullptr};
   QDataStream* _stream {nullptr};
   quint64 _usedSize {0};
   bool _good {false};
};



#endif
