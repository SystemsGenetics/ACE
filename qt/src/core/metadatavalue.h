#ifndef ACE_METADATA_H
#define ACE_METADATA_H
#include <QtCore>

#include "utilities.h"



namespace Ace
{
class Metadata
{
public:
   enum Type
   {
      Null
      ,Bool
      ,Double
      ,String
      ,Image
      ,Array
      ,Object
   };
   Metadata(Type type = Null);
   ~Metadata();
   ACE_DISBALE_COPY_AND_MOVE(Metadata)
   bool isNull() const;
   bool isBool() const;
   bool isDouble() const;
   bool isString() const;
   bool isImage() const;
   bool isArray() const;
   bool isObject() const;
   const bool& toBool() const;
   bool& toBool();
   const double& toDouble() const;
   double& toDouble();
   const QString& toString() const;
   QString& toString();
   const QImage& toImage() const;
   QImage& toImage();
   const QList<Metadata*>& toArray() const;
   QList<Metadata*>& toArray();
   const QMap<QString,Metadata*>& toObject() const;
   QMap<QString,Metadata*>& toObject();
   void setType(Type newType);
   Type getType() const;
   void clear();
private:
   void initialize(Type type);
   template<class T> T& toType(Type type);
   template<class T> const T& toType(Type type) const;
   QString getTypeName(Type type) const;
   Type _type {Null};
   void* _data {nullptr};
};
}



#endif
