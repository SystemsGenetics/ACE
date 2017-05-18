#ifndef DATAFACTORY_H
#define DATAFACTORY_H
#include <AceCore.h>



// Data factory
class DataFactory : public EAbstractDataFactory
{
public:
   // Make enumeration of all possible data types
   enum Types
   {
      Integers = 0
      ,Total
   };
   quint16 getCount() override final;
   QString getName(quint16 type) override final;
   QString getFileExtension(quint16 type) override final;
   std::unique_ptr<EAbstractData> make(quint16 type) override final;
};



#endif
