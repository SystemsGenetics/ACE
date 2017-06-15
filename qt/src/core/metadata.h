#ifndef ACE_METADATA_H
#define ACE_METADATA_H
#include <QtCore>



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
   using List = QList<Metadata*>;
   using Map = QList<QPair<QString,Metadata*>>;
   Metadata(Type type = Null);
   Metadata(const Metadata&) = delete;
   Metadata(Metadata&&) = delete;
   ~Metadata();
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
   const List& toArray() const;
   List& toArray();
   const Map& toObject() const;
   Map& toObject();
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
