#ifndef ACE_DATAOBJECT_H
#define ACE_DATAOBJECT_H
#include <QtCore>
#include "emetadata.h"
#include "global.h"
//



namespace Ace
{
   /*!
    * This is the file handler object for an open ACE data object. It works like a 
    * very basic Qt file device object but much simpler. Most importantly it hides the 
    * very beginning of the file which is the header of the data object, preventing it 
    * from accidentally being overwritten. This class is also unique in treating any 
    * read operation to its underlying file as constant. So things such as seeking and 
    * reading functions are declared constant. This is because ACE is made towards big 
    * data where it is impossible to just read an entire file into system memory. 
    */
   class DataObject : public QObject
   {
      Q_OBJECT
   public:
      explicit DataObject(const QString& path, QObject* parent = nullptr);
      explicit DataObject(const QString& path, quint16 type, const EMetadata& system, QObject* parent = nullptr);
      QString path() const;
      quint16 type() const;
      qint64 size() const;
      EMetadata systemMeta() const;
      EMetadata userMeta() const;
      void seek(qint64 index) const;
      void allocate(int size);
      const EDataStream& stream() const;
      EDataStream& stream();
      EAbstractData* data();
      void setUserMeta(const EMetadata& newRoot);
      void read(char* data, qint64 size) const;
      void write(const char* data, qint64 size);
   signals:
      /*!
       * Signals that the file of this data object has been overwritten by another data 
       * object being created. The owner of this data object must delete this data object 
       * immediately to avoid undefined behavior. 
       */
      void overwritten();
   public slots:
      void dataOverwritten(const QString& canonicalPath, Ace::DataObject* object);
   private:
      void openObject();
      void readHeader();
      void writeHeader();
      void makeData(const QString& name, const QString& extension);
      /*!
       * A special value that is written to the beginning of all data object files to 
       * identify it as an ACE data object file. 
       */
      constexpr static qint64 _specialValue {584};
      /*!
       * The data object type for this data object. 
       */
      quint16 _type;
      /*!
       * The absolute canonical path for this data object's file. 
       */
      QString _path;
      /*!
       * The system metadata for this object. If the system metadata has not been set yet 
       * then this is null. 
       */
      EMetadata _system;
      /*!
       * Pointer to this data object's qt file device. 
       */
      QFile* _file {nullptr};
      /*!
       * Pointer to this data object's data stream used for all input and output. 
       */
      EDataStream* _stream {nullptr};
      /*!
       * Pointer to this data object's abstract data object. 
       */
      EAbstractData* _data {nullptr};
      /*!
       * The offset of this data object's header data. This includes the system metadata. 
       * If this is a new file and the system metadata has not been written yet then this 
       * only includes basic header data. Once the system metadata is written this offset 
       * is updated to include it. 
       */
      qint64 _headerOffset;
   };
}



#endif
