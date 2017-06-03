#ifndef ACE_METADATAVALUE_H
#define ACE_METADATAVALUE_H
#include <QtCore>



namespace Ace
{
class MetadataArray;
class MetadataObject;



class MetadataValue
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
   MetadataValue(Type type = Null);
   ~MetadataValue();
   MetadataValue(const MetadataValue& copy);
   MetadataValue(MetadataValue&& move);
   MetadataValue& operator=(const MetadataValue& copy);
   MetadataValue& operator=(MetadataValue&& move);
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
   const MetadataArray& toArray() const;
   MetadataArray& toArray();
   const MetadataObject& toObject() const;
   MetadataObject& toObject();
   void setType(Type newType);
   Type getType() const;
   void clear();
private:
   void initialize(Type type);
   void checkType(Type type) const;
   void checkCopyOrMove(const QString& what) const;
   void makeCopy(const MetadataValue& copy);
   QString getTypeName(Type type) const;
   Type _type {Null};
   void* _data {nullptr};
};
}



#endif
