#ifndef MATHTRANSFORM_INPUT_H
#define MATHTRANSFORM_INPUT_H
#include <core/eabstractanalytic_input.h>
#include "mathtransform.h"
//



/*!
 */
class MathTransform::Input : public EAbstractAnalytic::Input
{
   Q_OBJECT
public:
   /*!
    */
   enum Argument
   {
      /*!
       */
      InputData
      /*!
       */
      ,OutputData
      /*!
       */
      ,OperationType
      /*!
       */
      ,Amount
      /*!
       */
      ,Total
   };
   explicit Input(MathTransform* parent);
   virtual int size() const override final;
   virtual EAbstractAnalytic::Input::Type type(int index) const override final;
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
    */
   static const QStringList _operationNames;
   /*!
    */
   MathTransform* _base;
};



#endif
