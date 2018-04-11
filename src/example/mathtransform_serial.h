#ifndef MATHTRANSFORM_SERIAL_H
#define MATHTRANSFORM_SERIAL_H
#include <core/eabstractanalytic_serial.h>
#include "mathtransform.h"
//



/*!
 */
class MathTransform::Serial : public EAbstractAnalytic::Serial
{
   Q_OBJECT
public:
   explicit Serial(MathTransform* parent);
   virtual std::unique_ptr<EAbstractAnalytic::Block> execute(const EAbstractAnalytic::Block* block) override final;
private:
   /*!
    */
   MathTransform* _base;
};



#endif
