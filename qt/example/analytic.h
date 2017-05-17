#ifndef ANALYTIC_H
#define ANALYTIC_H
#include <AceCore.h>



class Data;



class Analytic : public EAbstractAnalytic
{
   Q_OBJECT
public:
   enum Arguments
   {
      InputData = 0
      ,OutputData
      ,OperationType
      ,Amount
      ,Total
   };
   int getArgumentCount() override final;
   QString getCommandLineName(int argument) override final;
   ArgumentType getArgumentType(int argument) override final;
   QString getArgumentTitle(int argument) override final;
   QStringList getComboValues(int argument) override final;
   QString getArgumentWhatsThis(int argument) override final;
   QStringList getFileArgumentFilters(int argument) override final;
   quint16 getDataArgumentType(int argument) override final;
   void setArgument(int argument, QVariant value) override final;
   void setArgument(int argument, QFile* file) override final;
   void setArgument(int argument, EAbstractData* data) override final;
   bool initialize() override final;
   int getBlockSize() override final;
   bool runBlock(int block) override final;
   void finish() override final;
private:
   enum class OperType
   {
      Addition
      ,Subtraction
      ,Multiplication
      ,Division
   };
   Data* _in {nullptr};
   Data* _out {nullptr};
   OperType _type {OperType::Addition};
   int _amount {0};
};



#endif
