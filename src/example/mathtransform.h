#ifndef MATHTRANSFORM_H
#define MATHTRANSFORM_H
#include <core/core.h>
#include "global.h"
//



/*!
 */
class MathTransform : public EAbstractAnalytic
{
   Q_OBJECT
public:
   class Block;
   class Input;
   class Serial;
   virtual int size() const override final;
   virtual std::unique_ptr<EAbstractAnalytic::Block> makeBlock(int index) const override final;
   virtual void process(int index, const EAbstractAnalytic::Block* results) override final;
   virtual EAbstractAnalytic::Input* makeInput() override final;
   virtual EAbstractAnalytic::Serial* makeSerial() override final;
   virtual void initialize() override final;
private:
   /*!
    */
   enum class Operation
   {
      /*!
       */
      Addition
      /*!
       */
      ,Subtraction
      /*!
       */
      ,Multiplication
      /*!
       */
      ,Division
   };
   /*!
    */
   IntegerArray* _in {nullptr};
   /*!
    */
   IntegerArray* _out {nullptr};
   /*!
    */
   Operation _type {Operation::Addition};
   /*!
    */
   int _amount {0};
};



#endif
