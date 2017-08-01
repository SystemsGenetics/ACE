#ifndef ACE_METADATA_H
#define ACE_METADATA_H
#include <QtCore>



class EDataStream;



class EMetadata
{
public:
   /// All possible metadata types.
   enum Type
   {
      Null = 0 ///< Null type
      ,Bool ///< Boolean type
      ,Double ///< Real number type
      ,String ///< String type
      ,Bytes ///< Byte array(image) type
      ,Array ///< Array type
      ,Object ///< Object type
   };
   using List = QList<EMetadata*>;
   using Map = QMap<QString,EMetadata*>;
   /// create a new metadata object of given type.
   ///
   /// @param type Type.
   EMetadata(Type type = Null);
   EMetadata(const EMetadata& copy);
   EMetadata(EMetadata&&) = delete;
   ~EMetadata();
   /// Test if metadata object is of type Null.
   ///
   /// @return True if metadata is of type Null.
   bool isNull() const;
   /// Test if metadata object is of type Bool.
   ///
   /// @return True if metadata is of type Bool.
   bool isBool() const;
   /// Test if metadata object is of type Double.
   ///
   /// @return True if metadata is of type Double.
   bool isDouble() const;
   /// Test if metadata object is of type String.
   ///
   /// @return True if metadata is of type String.
   bool isString() const;
   /// Test if metadata object is of type Bytes.
   ///
   /// @return True if metadata is of type Bytes.
   bool isBytes() const;
   /// Test if metadata object is of type Array.
   ///
   /// @return True if metadata is of type Array.
   bool isArray() const;
   /// Test if metadata object is of type Object.
   ///
   /// @return True if metadata is of type Object.
   bool isObject() const;
   /// Get pointer to boolean value of metadata. WARNING metadata object must be of type Bool.
   ///
   /// @return Pointer to value.
   const bool* toBool() const;
   /// Get pointer to boolean value of metadata. WARNING metadata object must be of type Bool.
   ///
   /// @return Pointer to value.
   bool* toBool();
   /// Get pointer to double value of metadata. WARNING metadata object must be of type Double.
   ///
   /// @return Pointer to value.
   const double* toDouble() const;
   /// Get pointer to double value of metadata. WARNING metadata object must be of type Double.
   ///
   /// @return Pointer to value.
   double* toDouble();
   /// Get pointer to string value of metadata. WARNING metadata object must be of type String.
   ///
   /// @return Pointer to value.
   const QString* toString() const;
   /// Get pointer to string value of metadata. WARNING metadata object must be of type String.
   ///
   /// @return Pointer to value.
   QString* toString();
   /// Get pointer to byte array of metadata. WARNING metadata object must be of type Bytes.
   ///
   /// @return Pointer to value.
   const QByteArray* toBytes() const;
   /// Get pointer to boolean value of metadata. WARNING metadata object must be of type Bytes.
   ///
   /// @return Pointer to value.
   QByteArray* toBytes();
   /// Get pointer to array of metadata. WARNING metadata object must be of type Array.
   ///
   /// @return Pointer to value.
   const List* toArray() const;
   /// Get pointer to array of metadata. WARNING metadata object must be of type Array.
   ///
   /// @return Pointer to value.
   List* toArray();
   /// Get pointer to object of metadata. WARNING metadata object must be of type Object.
   ///
   /// @return Pointer to value.
   const Map* toObject() const;
   /// Get pointer to object of metadata. WARNING metadata object must be of type Object.
   ///
   /// @return Pointer to value.
   Map* toObject();
   /// Return value of metadata object as Qt variant object. WARNING metadata cannot be of type
   /// Array or Object.
   ///
   /// @return Variant object.
   QVariant toVariant() const;
   /// Set metadata to new type, clearing any data the object currently holds.
   ///
   /// @param newType New type.
   void setType(quint8 newType);
   /// Get metadata object's type.
   ///
   /// @return Type.
   quint8 getType() const;
   /// Set metadata to type Null, clearning any data the object currently holds.
   void clear();
   /// Internal command DO NOT USE.
   void setParent(EMetadata* parent);
   /// Internal command DO NOT USE.
   EMetadata* getParent() const;
   /// Internal command DO NOT USE.
   int getChildIndex(EMetadata* child) const;
   /// Get type name of metadata object.
   ///
   /// @return Type.
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
