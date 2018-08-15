#ifndef EMETADATA_H
#define EMETADATA_H
#include "global.h"



class QString;
class QByteArray;
class QJsonValue;
//



/*!
 * This holds a single metadata value. It is designed as a variant class which 
 * means it can hold any possible value type supported by the metadata system. 
 * Because this can be an array or object it can possibly hold additional metadata 
 * objects as children. Care should be taken to not attempt to convert a metadata 
 * object to a type that it is not. If the object attempts to convert itself to a 
 * type it is not an exception will be thrown. 
 */
class EMetadata
{
public:
   EMetadata& operator=(const EMetadata& object);
   EMetadata& operator=(EMetadata&& object);
public:
   /*!
    * Defines all possible types a metadata object can contain. 
    */
   enum Type
   {
      /*!
       * Defines the object is empty and contains no data. 
       */
      Null
      /*!
       * Defines the object is a Boolean. 
       */
      ,Bool
      /*!
       * Defines the object is a real number. 
       */
      ,Double
      /*!
       * Defines the object is a string. 
       */
      ,String
      /*!
       * Defines the object is an array of bytes. The QByteArray type is used to store 
       * this type. 
       */
      ,Bytes
      /*!
       * Defines the object is an array of additional metadata objects. 
       */
      ,Array
      /*!
       * Defines the object is a mapping of additional metadata objects. The mapping is 
       * done using strings as keywords. 
       */
      ,Object
   };
   EMetadata(Type type = Null);
   EMetadata(double value);
   EMetadata(const QString& value);
   EMetadata(const QByteArray& value);
   EMetadata(const EMetaArray& value);
   EMetadata(const EMetaObject& value);
   EMetadata(const QJsonValue& value);
   EMetadata(const EMetadata& object);
   EMetadata(EMetadata&& object);
   ~EMetadata();
   QJsonValue toJson() const;
   EMetadata::Type type() const;
   bool isNull() const;
   bool isBool() const;
   bool isDouble() const;
   bool isString() const;
   bool isBytes() const;
   bool isArray() const;
   bool isObject() const;
   const bool& toBool() const;
   const double& toDouble() const;
   const QString& toString() const;
   const QByteArray& toBytes() const;
   const EMetaArray& toArray() const;
   const EMetaObject& toObject() const;
   bool& toBool();
   double& toDouble();
   QString& toString();
   QByteArray& toBytes();
   EMetaArray& toArray();
   EMetaObject& toObject();
   static QString typeName(Type type);
private:
   void checkType(Type type) const;
   void clear();
   void create();
   void copy(const void* data);
   /*!
    * This stores what type this object contains. 
    */
   Type _type;
   /*!
    * This is a pointer to the actual data, if any, that this object contains. 
    */
   void* _data {nullptr};
};



#endif
