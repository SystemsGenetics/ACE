#ifndef EMETAOBJECT_H
#define EMETAOBJECT_H
#include <QMap>



class EMetadata;



/*!
 * This holds a mapping of metadata objects. It inherits from Qt so it is copy 
 * on write. The mapping is done using strings. Each unique key can only hold a 
 * single metadata value. Refer to EMetadata for more information about the 
 * metadata system. 
 */
class EMetaObject : private QMap<QString,EMetadata>
{
public:
   EMetaObject& operator=(const EMetaObject& object);
   bool isEmpty() const;
   int size() const;
   QMap<QString,EMetadata>::iterator begin();
   QMap<QString,EMetadata>::const_iterator begin() const;
   QMap<QString,EMetadata>::const_iterator cbegin() const;
   QMap<QString,EMetadata>::iterator end();
   QMap<QString,EMetadata>::const_iterator end() const;
   QMap<QString,EMetadata>::const_iterator cend() const;
   EMetadata at(const QString& key) const;
   void insert(const QString& key, const EMetadata& value);
   void clear();
   EMetadata& operator[](const QString& key);
   EMetadata operator[](const QString& key) const;
};



#endif
