#ifndef ACE_DATAOBJECT_H
#define ACE_DATAOBJECT_H
#include <QtCore>

#include "utilities.h"
#include "abstractdata.h"
#include "datastream.h"



class EDataStream;



namespace Ace
{
   class DataObject
   {
   public:
      enum Status
      {
         Ok = 0
         ,CannotOpen
         ,CannotWrite
         ,InvalidDataType
         ,CorruptFile
      };
      DataObject(const QString& path);
      ~DataObject();
      ACE_MAKE_NO_COPY_OR_MOVE(DataObject);
      bool seek(quint64 offset);
      bool allocate(quint64 size);
      Status getStatus() const;
      void clear(const QString& newType);
      bool isNew() const;
      EAbstractData& data();
      operator bool() const;
   private:
      constexpr static int _mininumFileSize {12};
      constexpr static quint64 _specialValue {584};
      std::unique_ptr<QFile> _file {nullptr};
      std::unique_ptr<EDataStream> _stream {nullptr};
      std::unique_ptr<EAbstractData> _data {nullptr};
      quint64 _headerOffset {0};
      Status _status {Ok};
      bool _isNew {true};
   };
}



#endif
// 64special 16inttype qstring
