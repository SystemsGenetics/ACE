#ifndef ACE_METADATA_H
#define ACE_METADATA_H
#include <QtCore>



class EDataStream;



namespace Ace
{
class Metadata
{
public:
   enum Errors
   {
      TypeMismatch = 0
      ,CannotWrite
      ,CannotRead
   };
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
   using List = QList<Metadata*>;
   using Map = QMap<QString,Metadata*>;
   Metadata(Type type = Null);
   Metadata(const Metadata&) = delete;
   Metadata(Metadata&&) = delete;
   ~Metadata();
   bool isNull() const;
   bool isBool() const;
   bool isDouble() const;
   bool isString() const;
   bool isBytes() const;
   bool isArray() const;
   bool isObject() const;
   const bool& toBool() const;
   bool& toBool();
   const double& toDouble() const;
   double& toDouble();
   const QString& toString() const;
   QString& toString();
   const QByteArray& toBytes() const;
   QByteArray& toBytes();
   const List& toArray() const;
   List& toArray();
   const Map& toObject() const;
   Map& toObject();
   QVariant toVariant() const;
   void setType(quint8 newType);
   quint8 getType() const;
   void clear();
   void setParent(Metadata* parent);
   Metadata* getParent() const;
   int getChildIndex(Metadata* child) const;
   QString getTypeName() const;
   friend EDataStream& operator>>(EDataStream& stream, Metadata& meta);
   friend EDataStream& operator<<(EDataStream& stream, Metadata& meta);
private:
   void initialize(quint8 type);
   template<class T> T& toType(quint8 type);
   template<class T> const T& toType(quint8 type) const;
   QString convertTypeName(quint8 type) const;
   quint8 _type {Null};
   void* _data {nullptr};
   Metadata* _parent {nullptr};
};



EDataStream& operator>>(EDataStream& stream, Metadata& meta);
EDataStream& operator<<(EDataStream& stream, Metadata& meta);
} // namespace Ace



#endif
