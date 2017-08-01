#ifndef EABSTRACTDATA_H
#define EABSTRACTDATA_H
#include <QtCore>
#include <memory>

#include "utilities.h"



namespace Ace { class DataObject; }
class EDataStream;
class EMetadata;



/// Interface class that defines a data object type.
class EAbstractData
{
public:
   EAbstractData() = default;
   virtual ~EAbstractData() = default;
   ACE_DISBALE_COPY_AND_MOVE(EAbstractData)
   /// Read data for object.
   virtual void readData() = 0;
   /// Report offset where data for object ends.
   ///
   /// @return End of data offset.
   virtual quint64 getDataEnd() const = 0;
   /// Initialize a new data object..
   virtual void newData() = 0;
   /// Prepare a new data object for input.
   ///
   /// @param preAllocate Allocate space needed for data that will be given to new data object if
   /// true.
   virtual void prepare(bool preAllocate) = 0;
   /// Finalize new data object after being given all input.
   virtual void finish() = 0;
   /// Internal command DO NOT USE.
   virtual QAbstractItemModel* getModel() = 0;
   /// Internal command DO NOT USE.
   void initialize(Ace::DataObject* object, EDataStream* stream) noexcept;
protected:
   /// Get reference to root metadata object.
   ///
   /// @return Reference to metadata root.
   EMetadata& meta();
   /// Get reference to data stream of object's file.
   ///
   /// @return Reference to data stream.
   EDataStream& stream();
   /// Seek to new offset within data object's file.
   ///
   /// @param offset New offset.
   /// @return True if successful or false if error occured.
   bool seek(quint64 offset) noexcept;
   /// Allocate new space in object's file in bytes based off current seek value. If new size is
   /// smaller than current size of file it will be shrinked.
   ///
   /// @param size Space to allocate in bytes.
   /// @return True if successful or false if error occured.
   bool allocate(quint64 size) noexcept;
private:
   Ace::DataObject* _data {nullptr};
   EDataStream* _stream {nullptr};
   EMetadata* _meta {nullptr};
};



#endif
// ->newData
// start analytic
// run initialize
// ->prepare
// run analytic
// ->finish
// ->getDataEnd
