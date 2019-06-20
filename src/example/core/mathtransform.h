#ifndef MATHTRANSFORM_H
#define MATHTRANSFORM_H
#include <core/core.h>
#include "dataframe.h"
//



/*!
 * This class implements a math transform analytic. This analytic takes in a
 * dataframe and outputs a second dataframe. Each value in the dataframe is 
 * transformed with a given mathematical operation and amount and then written to 
 * the output dataframe. The supported math operations are add, subtract, 
 * multiply, and divide. This analytic fully supports both serial and OpenCL. 
 */
class MathTransform : public EAbstractAnalytic
{
   Q_OBJECT
public:
   class Input;
   class Block;
   class Serial;
   class OpenCL;
   class CUDA;
   virtual int size() const override final;
   virtual std::unique_ptr<EAbstractAnalyticBlock> makeWork(int index) const override final;
   virtual std::unique_ptr<EAbstractAnalyticBlock> makeWork() const override final;
   virtual std::unique_ptr<EAbstractAnalyticBlock> makeResult() const override final;
   virtual void process(const EAbstractAnalyticBlock* result) override final;
   virtual EAbstractAnalyticInput* makeInput() override final;
   virtual EAbstractAnalyticSerial* makeSerial() override final;
   virtual EAbstractAnalyticOpenCL* makeOpenCL() override final;
   virtual EAbstractAnalyticCUDA* makeCUDA() override final;
   virtual void initialize() override final;
   virtual void initializeOutputs() override final;
private:
   /*!
    * Defines the mathematical operations this analytic supports. 
    */
   enum class Operation
   {
      /*!
       * Defines the addition operation. 
       */
      Addition
      /*!
       * Defines the subtraction operation. 
       */
      ,Subtraction
      /*!
       * Defines the multiplication operation. 
       */
      ,Multiplication
      /*!
       * Defines the division operation. 
       */
      ,Division
   };
   /*!
    * Pointer to the input dataframe for this analytic. 
    */
   DataFrame* _in {nullptr};
   /*!
    * Pointer to the output dataframe for this analytic. In some cases an analytic 
    * will be initialized without any of its output set. These analytic objects will 
    * never have their process interface called by ACE. 
    */
   DataFrame* _out {nullptr};
   /*!
    * The mathematical operation this analytic does to all input integers. 
    */
   Operation _type {Operation::Addition};
   /*!
    * The amount used with the mathematical operation by this analytic on all input 
    * integers. 
    */
   int _amount {0};
};



#endif
