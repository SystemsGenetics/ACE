#ifndef EABSTRACTDATAFACTORY_H
#define EABSTRACTDATAFACTORY_H
#include <memory>

#include "utilities.h"



class EAbstractData;



/// Factory object that informs ACE of all possible data types and makes them.
class EAbstractDataFactory
{
public:
   EAbstractDataFactory() = default;
   virtual ~EAbstractDataFactory() = default;
   EAbstractDataFactory(const EAbstractDataFactory&) = delete;
   /// Get instance of data factory.
   ///
   /// @return Reference to factory.
   static EAbstractDataFactory& instance();
   /// Set new data factory instance.
   ///
   /// @param factory New data factory.
   static void setInstance(std::unique_ptr<EAbstractDataFactory>&& factory);
   /// Get total number of possible data types.
   virtual quint16 size() = 0;
   /// Get name of specific data type.
   ///
   /// @param type Data type identifier.
   virtual QString name(quint16 type) = 0;
   /// Get file extension of specific data type.
   ///
   /// @param type Data type identifier.
   virtual QString fileExtension(quint16 type) = 0;
   /// Make new data object of given type.
   ///
   /// @param type Data type identifier.
   virtual std::unique_ptr<EAbstractData> make(quint16 type) = 0;
private:
   static std::unique_ptr<EAbstractDataFactory> _instance;
};



#endif
