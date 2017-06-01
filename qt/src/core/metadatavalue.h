#ifndef ACE_METADATAVALUE_H
#define ACE_METADATAVALUE_H
#include <QtCore>
#include <memory>



namespace Ace
{
   class MetadataArray;
   class MetadataObject;
}



namespace Ace
{
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
      MetadataValue();
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
      template<class T> const T& get() const;//implement only supported types!
      template<class T> T& get();
      void setType(Type newType);
      Type getType();
   };
}



#endif
