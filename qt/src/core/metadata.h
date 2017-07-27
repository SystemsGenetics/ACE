#ifndef ACE_METADATA_H
#define ACE_METADATA_H
#include <QtCore>



class EDataStream;



class EMetadata
{
public:
   enum Type
   {
      Null = 0
      ,Bool
      ,Double
      ,String
      ,Bytes
      ,Array
      ,Object
   };
   using List = QList<EMetadata*>;
   using Map = QMap<QString,EMetadata*>;
   EMetadata(Type type = Null);
   EMetadata(const EMetadata& copy);
   EMetadata(EMetadata&&) = delete;
   ~EMetadata();
   bool isNull() const;
   bool isBool() const;
   bool isDouble() const;
   bool isString() const;
   bool isBytes() const;
   bool isArray() const;
   bool isObject() const;
   const bool* toBool() const;
   bool* toBool();
   const double* toDouble() const;
   double* toDouble();
   const QString* toString() const;
   QString* toString();
   const QByteArray* toBytes() const;
   QByteArray* toBytes();
   const List* toArray() const;
   List* toArray();
   const Map* toObject() const;
   Map* toObject();
   QVariant toVariant() const;
   void setType(quint8 newType);
   quint8 getType() const;
   void clear();
   void setParent(EMetadata* parent);
   EMetadata* getParent() const;
   int getChildIndex(EMetadata* child) const;
   QString getTypeName() const;
   friend EDataStream& operator>>(EDataStream& stream, EMetadata& meta);
   friend EDataStream& operator<<(EDataStream& stream, EMetadata& meta);
private:
   void initialize(quint8 type);
   template<class T> T* toType(quint8 type);
   template<class T> const T* toType(quint8 type) const;
   QString convertTypeName(quint8 type) const;
   quint8 _type {Null};
   void* _data {nullptr};
   EMetadata* _parent {nullptr};
};



EDataStream& operator>>(EDataStream& stream, EMetadata& meta);
EDataStream& operator<<(EDataStream& stream, EMetadata& meta);



#endif
