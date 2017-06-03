#ifndef ACE_METADATAOBJECT_H
#define ACE_METADATAOBJECT_H
#include <QtCore>



namespace Ace
{
class MetadataValue;



class MetadataObject
{
public:
   class Iterator;
   Iterator insert(const QString& key, const MetadataValue& value);
   Iterator move(const QString& key, MetadataValue&& value);
   bool remove(const QString& key);
   Iterator begin();
   Iterator end();
   Iterator find(const QString& key);
   Iterator erase(Iterator iterator);
   const MetadataValue& at(const QString& key) const;
   MetadataValue& operator[](const QString& key);
private:
   QMap<QString,MetadataValue> _map;
};



class MetadataObject::Iterator
{
public:
   friend class MetadataObject;
   const QString& key() const;
   MetadataValue& operator*();
   void operator++();
   bool operator!=(const Iterator& compare) const;
private:
   using Pointer = QMap<QString,MetadataValue>::iterator;
   Iterator(Pointer i);
   Pointer _i;
};
}



#endif
