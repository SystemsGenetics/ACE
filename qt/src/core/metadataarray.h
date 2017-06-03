#ifndef ACE_METADATAARRAY_H
#define ACE_METADATAARRAY_H
#include <QtCore>
#include <memory>



namespace Ace
{
class MetadataValue;



class MetadataArray
{
public:
   class Iterator;
   MetadataArray();
   ~MetadataArray();
   void pushFront(const MetadataValue& value);
   void pushFront(MetadataValue&& value);
   void pushBack(const MetadataValue& value);
   void pushBack(MetadataValue&& value);
   bool popFront();
   bool popBack();
   bool remove(int indent);
   MetadataValue& front();
   MetadataValue& back();
   Iterator begin();
   Iterator end();
   Iterator erase(Iterator iterator);
   const MetadataValue& at(int indent) const;
   MetadataValue& operator[](int indent);
private:
   QList<MetadataValue> _list;
};



class MetadataArray::Iterator
{
public:
   friend class MetadataArray;
   MetadataValue& operator*();
   void operator++();
   bool operator!=(const Iterator& compare) const;
private:
   using Pointer = QList<std::unique_ptr<MetadataValue>>::iterator;
   Iterator(Pointer i);
   Pointer _i;
};
}



#endif
