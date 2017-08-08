#ifndef EXPORTINTEGERARRAY_H
#define EXPORTINTEGERARRAY_H
#include <core/AceCore.h>



class IntegerArray;



class ExportIntegerArray : public EAbstractAnalytic
{
public:
   // make enumeration of all the analytic's arguments
   enum Arguments
   {
      InputData = 0
      ,OutputFile
      ,Total
   };
   int getArgumentCount() override final { return Total; }
   ArgumentType getArgumentData(int argument) override final;
   QVariant getArgumentData(int argument, Role role) override final;
   void setArgument(int /*argument*/, QVariant /*value*/) override final {}
   void setArgument(int argument, QFile* file) override final;
   void setArgument(int argument, EAbstractData* data) override final;
   bool initialize() override final;

   // return block size of 1
   int getBlockSize() override final { return 1; }
   bool runBlock(int block) override final;
   void finish() override final {}
private:
   IntegerArray* _input;
   QFile* _output;
};



#endif
