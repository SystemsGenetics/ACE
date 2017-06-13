#ifndef DATAFACTORY_H
#define DATAFACTORY_H
#include <core/AceCore.h>



// Data factory
class DataFactory : public EAbstractDataFactory
{
public:
   // Make enumeration of all possible data types
   enum Types
   {
      IntegerArrayType = 0
      ,Total
   };
   quint16 getCount() noexcept override final;
   QString getName(quint16 type) noexcept override final;
   QString getFileExtension(quint16 type) noexcept override final;
   std::unique_ptr<EAbstractData> make(quint16 type) noexcept override final;
};



#endif
