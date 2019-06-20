#ifndef MATHTRANSFORM_INPUT_H
#define MATHTRANSFORM_INPUT_H
#include "mathtransform.h"
//



/*!
 * This implements the input class for its math transform parent analytic. 
 */
class MathTransform::Input : public EAbstractAnalyticInput
{
   Q_OBJECT
public:
   /*!
    * Defines all arguments for its parent analytic. 
    */
   enum Argument
   {
      /*!
       * Defines the input data argument. 
       */
      InputData
      /*!
       * Defines the output data argument. 
       */
      ,OutputData
      /*!
       * Defines the operation type argument. 
       */
      ,OperationType
      /*!
       * Defines the amount argument. 
       */
      ,Amount
      /*!
       * Defines the total number of arguments this input class contains. 
       */
      ,Total
   };
   explicit Input(MathTransform* parent);
   virtual int size() const override final;
   virtual EAbstractAnalyticInput::Type type(int index) const override final;
   virtual QVariant data(int index, Role role) const override final;
   virtual void set(int index, const QVariant& value) override final;
   virtual void set(int index, QFile* file) override final;
   virtual void set(int index, EAbstractData* data) override final;
private:
   QVariant inputDataData(Role role) const;
   QVariant outputDataData(Role role) const;
   QVariant operationTypeData(Role role) const;
   QVariant amountData(Role role) const;
   /*!
    * String list of mathematical operations for this analytic that correspond exactly 
    * to its enumeration. Used for handling the operation type argument for this input 
    * object. 
    */
   static const QStringList _operationNames;
   /*!
    * Pointer to this input object's parent math transform analytic. 
    */
   MathTransform* _base;
};



#endif
