#ifndef IMPORTINTEGERARRAY_H
#define IMPORTINTEGERARRAY_H
#include <core/AceCore.h>



class IntegerArray;



class ImportIntegerArray : public EAbstractAnalytic
{
public:
   // make enumeration of all analytic's arguments
   enum Arguments
   {
      InputFile = 0
      ,OutputData
      ,Total
   };
   int getArgumentCount() override final { return Total; }
   ArgumentType getArgumentData(int argument) override final;
   QVariant getArgumentData(int argument, Role role) override final;
   void setArgument(int /*argument*/, QVariant /*value*/) override final {}
   void setArgument(int argument, QFile* file) override final;
   void setArgument(int argument, EAbstractData* data) override final;
   quint32 getCapabilities() const override final { return Capabilities::Serial; }
   bool initialize() override final;
   void runSerial() override final;
   int getBlockSize() override final { return 1; }
   bool runBlock(int /*block*/) override final { return false; }
   void finish() override final {}
private:
   QFile* _input;
   IntegerArray* _output;
};



#endif
