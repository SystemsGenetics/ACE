#include "metadataobject.h"
#include "metadatavalue.h"






Ace::MetadataObject::Iterator Ace::MetadataObject::insert(const QString& key
                                                          , const MetadataValue& value)
{
   return Iterator(_map.insert(key,value));
}






Ace::MetadataObject::Iterator Ace::MetadataObject::move(const QString& key, MetadataValue&& value)
{
   auto i = _map.insert(key,MetadataValue());
   *i = std::move(value);
   return i;
}






bool Ace::MetadataObject::remove(const QString& key)
{
   return _map.remove(key) > 0;
}






Ace::MetadataObject::Iterator Ace::MetadataObject::begin()
{
   return Iterator(_map.begin());
}






Ace::MetadataObject::Iterator Ace::MetadataObject::end()
{
   return Iterator(_map.end());
}






Ace::MetadataObject::Iterator Ace::MetadataObject::find(const QString& key)
{
   return Iterator(_map.find(key));
}






Ace::MetadataObject::Iterator Ace::MetadataObject::erase(Iterator iterator)
{
   return Iterator(_map.erase(iterator._i));
}






const Ace::MetadataValue& Ace::MetadataObject::at(const QString& key) const
{
   auto i = _map.find(key);
   return *i;
}






Ace::MetadataValue& Ace::MetadataObject::operator[](const QString& key)
{
   return _map[key];
}






const QString &Ace::MetadataObject::Iterator::key() const
{
   return _i.key();
}






Ace::MetadataValue& Ace::MetadataObject::Iterator::operator*()
{
   return *_i;
}






void Ace::MetadataObject::Iterator::operator++()
{
   ++_i;
}






bool Ace::MetadataObject::Iterator::operator!=(const Iterator& compare) const
{
   return _i != compare._i;
}






Ace::MetadataObject::Iterator::Iterator(Pointer i):
   _i(i)
{}
