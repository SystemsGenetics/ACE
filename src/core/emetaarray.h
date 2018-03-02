#ifndef EMETAARRAY_H
#define EMETAARRAY_H
#include <QList>
#include "emetadata.h"



/*!
 * This holds a list of metadata objects. It inherits from Qt so it is copy on 
 * write. Refer to EMetadata for more information about the metadata system. 
 */
class EMetaArray : public QList<EMetadata>
{
public:
   bool isEmpty() const;
   int size() const;
   QList<EMetadata>::iterator begin();
   QList<EMetadata>::const_iterator begin() const;
   QList<EMetadata>::const_iterator cbegin() const;
   QList<EMetadata>::iterator end();
   QList<EMetadata>::const_iterator end() const;
   QList<EMetadata>::const_iterator cend() const;
   const EMetadata& at(int index) const;
   void insert(int index, const EMetadata& value);
   void append(const EMetadata& value);
   void clear();
   EMetadata& operator[](int index);
   const EMetadata& operator[](int index) const;
   EMetaArray& operator<<(const EMetadata& value);
};



#endif
